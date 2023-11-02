#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

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
