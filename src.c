#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

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

// Interpret bytecode.
void interpret() {
	// Array of memory cells. Contains 2^16 cells to take advantage of wrapping.
	uint8_t *cells = calloc(UINT16_MAX + 1, sizeof(uint8_t));
	
	if (cells == NULL) {
		error("Could not allocate memory cells.");
	}
	
	// Data pointer. The index of the current memory cell.
	uint16_t dp = 0;
	
	free(cells);
}

// Test Brainiac.
int main() {
	interpret();
	printf("Hello, Brainiac!\n");
}
