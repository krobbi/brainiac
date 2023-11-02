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

// Exit with an error message.
void error(const char *message) {
	fprintf(stderr, "%s\n", message);
	exit(EXIT_FAILURE);
}

// Compile bytecode from a path.
uint8_t *compile(const char *path) {
	printf("Compiling from path: '%s'\n", path);
	
	// Test bytecode.
	uint8_t *bytecode = (uint8_t*)malloc(9);
	
	if (bytecode == NULL) {
		error("Could not allocate test bytecode.");
	}
	
	// Shuffle operations to test dispatch.
	bytecode[0] = OP_RIGHT;
	bytecode[1] = OP_INCREMENT;
	bytecode[2] = OP_LEFT;
	bytecode[3] = OP_DECREMENT;
	bytecode[4] = OP_OUTPUT;
	bytecode[5] = OP_BEGIN;
	bytecode[6] = OP_INPUT;
	bytecode[7] = OP_END;
	bytecode[8] = OP_HALT; // Bytecode must end with a halt operation.
	
	return bytecode;
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
			printf("Halt.\n");
			free(cells);
			return;
		}
		
		VM_OP(OP_RIGHT) {
			printf("Right.\n");
			++dp;
			VM_DISPATCH;
		}
		
		VM_OP(OP_LEFT) {
			printf("Left.\n");
			--dp;
			VM_DISPATCH;
		}
		
		VM_OP(OP_INCREMENT) {
			printf("Increment.\n");
			++cells[dp];
			VM_DISPATCH;
		}
		
		VM_OP(OP_DECREMENT) {
			printf("Decrement.\n");
			--cells[dp];
			VM_DISPATCH;
		}
		
		VM_OP(OP_OUTPUT) {
			printf("Output (TODO.)\n");
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
	
	// Bytecode to test the interpreter with.
	uint8_t *bytecode = compile(argv[1]);
	interpret(bytecode);
	free(bytecode);
}
