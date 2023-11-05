#include <stdio.h>
#include <stdlib.h>

#include "compiler.h"
#include "vm.h"

// Interpret bytecode from a source path and return an exit code.
static int interpretPath(const char *path) {
	uint8_t *bytecode = compilePath(path);
	
	if (bytecode != NULL) {
		int exitCode = interpret(bytecode);
		free(bytecode);
		return exitCode;
	} else {
		return EXIT_FAILURE;
	}
}

// Test Brainiac.
int main(int argc, const char *argv[]) {
	if (argc == 2) {
		return interpretPath(argv[1]);
	} else {
		fprintf(stderr, "Usage: brainiac <path>\n");
		return EXIT_FAILURE;
	}
}
