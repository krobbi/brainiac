#include <stdarg.h>
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
	
	// The compiler's loop depth.
	int depth;
} Compiler;

// Exit with an error message.
void error(const char *format, ...) {
	va_list args;
	va_start(args, format);
	vfprintf(stderr, format, args);
	va_end(args);
	fputc('\n', stderr);
	exit(EXIT_FAILURE);
}

// Initialize a compiler.
void compilerInit(Compiler *compiler) {
	compiler->count = 0;
	compiler->capacity = 8;
	compiler->bytecode = (uint8_t*)malloc(compiler->capacity * sizeof(uint8_t));
	compiler->depth = 0;
	
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

// Parse a character with a compiler.
void compilerParse(Compiler *compiler, char character) {
	// All non-command characters are comments.
	switch (character) {
		case '>': compilerEmit(compiler, OP_RIGHT); break;
		case '<': compilerEmit(compiler, OP_LEFT); break;
		case '+': compilerEmit(compiler, OP_INCREMENT); break;
		case '-': compilerEmit(compiler, OP_DECREMENT); break;
		case '.': compilerEmit(compiler, OP_OUTPUT); break;
		case ',': compilerEmit(compiler, OP_INPUT); break;
		case '[': {
			compilerEmit(compiler, OP_BEGIN);
			compiler->depth++;
			break;
		}
		case ']': {
			if (compiler->depth > 0) {
				compilerEmit(compiler, OP_END);
				compiler->depth--;
			} else {
				error("Cannot use ']' without an opening '['.");
			}
			
			break;
		}
	}
}

// End a compiler and pass ownership of its bytecode.
uint8_t *compilerEnd(Compiler *compiler) {
	if (compiler->depth > 0) {
		error("Cannot use '[' without a closing ']'.");
	}
	
	compilerEmit(compiler, OP_HALT);
	
	if (compiler->capacity > compiler->count) {
		compilerResize(compiler, compiler->count);
	}
	
	return compiler->bytecode;
}

// Compile bytecode from a path.
uint8_t *compile(const char *path) {
	// The file to compile.
	FILE *file = fopen(path, "rb");
	
	if (file == NULL) {
		error("Could not read '%s', file may not exist.", path);
	}
	
	// The compiler to compile the file with.
	Compiler compiler;
	compilerInit(&compiler);
	
	// The character to parse.
	int character;
	
	while ((character = fgetc(file)) != EOF) {
		compilerParse(&compiler, (char)character);
	}
	
	if (ferror(file) || !feof(file)) {
		error("Could not finish reading '%s'.", path);
	}
	
	if (fclose(file) == EOF) {
		error("Could not close '%s' after reading.", path);
	}
	
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
			int character = getchar();
			cells[dp] = character != EOF ? (uint8_t)character : 0;
			VM_DISPATCH;
		}
		
		// TODO: Optimize loops by including offset operands.
		VM_OP(OP_BEGIN) {
			if (!cells[dp]) {
				int depth = 1;
				
				do {
					switch (*ip++) {
						case OP_BEGIN: ++depth; break;
						case OP_END: --depth; break;
					}
				} while (depth);
			}
			
			VM_DISPATCH;
		}
		
		VM_OP(OP_END) {
			if (cells[dp]) {
				int depth = 1;
				--ip;
				
				do {
					switch (*--ip) {
						case OP_BEGIN: --depth; break;
						case OP_END: ++depth; break;
					}
				} while (depth);
			}
			
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
