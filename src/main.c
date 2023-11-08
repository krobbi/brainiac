#include <stdio.h>
#include <stdlib.h>

#include "compiler.h"
#include "vm.h"

// Test the compiler.
int main(int argc, const char *argv[]) {
	if (argc == 2) {
		uint8_t *bytecode = compilePath(argv[1]);
		
		if (bytecode == NULL) {
			return EXIT_FAILURE;
		}
		
		interpretBytecode(bytecode);
		free(bytecode);
		return EXIT_SUCCESS;
	} else {
		fprintf(stderr, "Usage: brainiac <path>\n");
		return EXIT_FAILURE;
	}
}
