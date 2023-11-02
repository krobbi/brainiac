#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

// Computed goto requires a GCC extension.
#ifdef __GNUC__
	#define USE_COMPUTED_GOTO
#endif

// A bytecode opcode.
enum {
	OP_HALT, // Halt execution. Equivalent to end of file.
	OP_RIGHT, // Increment the data pointer. Equivalent to '>'.
	OP_LEFT, // Decrement the data pointer. Equivalent to '<'.
	OP_INCREMENT, // Increment the pointed data. Equivalent to '+'.
	OP_DECREMENT, // Decrement the pointed data. Equivalent to '-'.
	OP_OUTPUT, // Output the pointed data as a character. Equivalent to '.'.
	OP_INPUT, // Input a character to the pointed data. Equivalent to ','.
	OP_BEGIN, // Jump forward if the pointed data is zero. Emitted from '['.
	OP_END, // Jump back if the pointed data is non-zero. Emitted from ']'.
};

// A compiler's state.
typedef struct {
	// The number of bytes in the compiler's bytecode.
	int count;
	
	// The capacity of the compiler's bytecode.
	int capacity;
	
	// The compiler's bytecode.
	uint8_t *bytecode;
} Compiler;

// Exit with an error message.
void error(const char *message) {
	fprintf(stderr, "%s\n", message);
	exit(EXIT_FAILURE);
}

// Initialize a compiler.
void compilerInit(Compiler *compiler) {
	compiler->count = 0;
	compiler->capacity = 8;
	compiler->bytecode = (uint8_t*)malloc(compiler->capacity * sizeof(uint8_t));
	
	if (compiler->bytecode == NULL) {
		error("Could not allocate bytecode.");
	}
}

// Resize a compiler's bytecode.
void compilerResize(Compiler *compiler, int capacity) {
	compiler->capacity = capacity;
	compiler->bytecode = (uint8_t*)realloc(compiler->bytecode, capacity * sizeof(uint8_t));
	
	if (compiler->bytecode == NULL) {
		error("Could not reallocate bytecode.");
	}
}

// Emit a byte of bytecode to a compiler.
void compilerEmit(Compiler *compiler, uint8_t byte) {
	if (compiler->count == compiler->capacity) {
		compilerResize(compiler, compiler->capacity * 2);
	}
	
	compiler->bytecode[compiler->count++] = byte;
}

// End a compiler and pass ownership of its bytecode.
uint8_t *compilerEnd(Compiler *compiler) {
	compilerEmit(compiler, OP_HALT);
	
	if (compiler->capacity > compiler->count) {
		compilerResize(compiler, compiler->count);
	}
	
	return compiler->bytecode;
}

// Compile a test character.
void compileTestChar(Compiler *compiler, char character) {
	int code = (int)(unsigned char)character;
	
	for (int i = 0; i < code; i++) {
		compilerEmit(compiler, OP_INCREMENT);
	}
	
	compilerEmit(compiler, OP_OUTPUT);
	compilerEmit(compiler, OP_RIGHT);
}

// Compile bytecode from a path.
uint8_t *compile(const char *path) {
	printf("Compiling '%s'...\n", path);
	
	// The compiler to emit bytecode with.
	Compiler compiler;
	compilerInit(&compiler);
	compileTestChar(&compiler, 'b');
	compileTestChar(&compiler, 'f');
	compileTestChar(&compiler, '!');
	compileTestChar(&compiler, '\n');
	return compilerEnd(&compiler);
}

// Interpret bytecode from an instruction pointer.
void interpret(uint8_t *ip) {
	// Array of memory cells. Contains 2^16 cells to take advantage of wrapping.
	uint8_t *cells = (uint8_t *)calloc(UINT16_MAX + 1, sizeof(uint8_t));
	
	if (cells == NULL) {
		error("Could not allocate memory cells.");
	}
	
	// Data pointer. The index of the current memory cell.
	uint16_t dp = 0;
	
	#ifdef USE_COMPUTED_GOTO
		// Table of addresses to jump to for each opcode.
		static void *dispatchTable[] = {
			[OP_HALT] = &&vm_OP_HALT,
			[OP_RIGHT] = &&vm_OP_RIGHT,
			[OP_LEFT] = &&vm_OP_LEFT,
			[OP_INCREMENT] = &&vm_OP_INCREMENT,
			[OP_DECREMENT] = &&vm_OP_DECREMENT,
			[OP_OUTPUT] = &&vm_OP_OUTPUT,
			[OP_INPUT] = &&vm_OP_INPUT,
			[OP_BEGIN] = &&vm_OP_BEGIN,
			[OP_END] = &&vm_OP_END,
		};
		
		#define VM_OP(opcode) vm_##opcode:
		#define VM_DISPATCH goto *dispatchTable[*ip++]
		#define VM_LOOP VM_DISPATCH;
	#else
		#define VM_LOOP for(;;) switch(*ip++)
		#define VM_OP(opcode) case opcode:
		#define VM_DISPATCH break
	#endif
	
	VM_LOOP {
		VM_OP(OP_HALT) {
			free(cells);
			return;
		}
		
		VM_OP(OP_RIGHT) {
			++dp;
			VM_DISPATCH;
		}
		
		VM_OP(OP_LEFT) {
			--dp;
			VM_DISPATCH;
		}
		
		VM_OP(OP_INCREMENT) {
			++cells[dp];
			VM_DISPATCH;
		}
		
		VM_OP(OP_DECREMENT) {
			--cells[dp];
			VM_DISPATCH;
		}
		
		VM_OP(OP_OUTPUT) {
			putchar(cells[dp]);
			VM_DISPATCH;
		}
		
		VM_OP(OP_INPUT) {
			printf("Input (TODO.)\n");
			VM_DISPATCH;
		}
		
		VM_OP(OP_BEGIN) {
			printf("Begin (TODO.)\n");
			VM_DISPATCH;
		}
		
		VM_OP(OP_END) {
			printf("End (TODO.)\n");
			VM_DISPATCH;
		}
	}
	
	#undef VM_LOOP
	#undef VM_OP
	#undef VM_DISPATCH
}

// Test Brainiac.
int main(int argc, const char *argv[]) {
	if (argc != 2) {
		error("Usage: brainiac <path>");
	}
	
	// Compiled bytecode to execute with the interpreter.
	uint8_t *bytecode = compile(argv[1]);
	interpret(bytecode);
	free(bytecode);
}
