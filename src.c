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
	
	// Test underflow and overflow.
	++cells[dp]; // Set cell 0 to 1.
	++cells[dp]; // Set cell 0 to 2.
	--dp; // Move to cell 65535.
	--dp; // Move to cell 65534.
	
	if (dp != 65534) {
		error("Data pointer underflow test failed.");
	}
	
	--cells[dp]; // Set cell 65534 to 255.
	--cells[dp]; // Set cell 65534 to 254.
	
	if (cells[dp] != 254) {
		error("Cell underflow test failed.");
	}
	
	++cells[dp]; // Set cell 65534 to 255.
	
	if (cells[dp] != 255) {
		error("Cell limit test failed.");
	}
	
	++cells[dp]; // Set cell 65534 to 0.
	--cells[dp]; // Set cell 65534 to 255.
	++cells[dp]; // Set cell 65534 to 0.
	++cells[dp]; // Set cell 65534 to 1.
	
	if (cells[dp] != 1) {
		error("Cell overflow test failed");
	}
	
	++dp; // Move to cell 65535.
	
	if (dp != 65535) {
		error("Data pointer limit test failed.");
	}
	
	++dp; // Move to cell 0.
	--dp; // Move to cell 65535.
	++dp; // Move to cell 0.
	
	if (dp != 0) {
		error("Data pointer overflow test failed.");
	}
	
	if (cells[dp] != 2) {
		error("Memory test failed.");
	}
	
	free(cells);
}

// Test Brainiac.
int main() {
	interpret();
	printf("Hello, Brainiac!\n");
}
