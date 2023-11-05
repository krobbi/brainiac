#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "compiler.h"
#include "vm.h"

// Run a REPL and return an exit code.
static int repl() {
	printf("Brainiac REPL - Enter Brainfuck code or 'exit' to exit:\n\n");
	char input[1024];
	
	for (;;) {
		printf("bf: ");
		
		if (fgets(input, sizeof(input), stdin) == NULL || memcmp(input, "exit", 4) == 0) {
			return EXIT_SUCCESS;
		}
		
		uint8_t *bytecode = compileSource(input);
		
		if (bytecode != NULL) {
			interpretBytecode(bytecode);
			free(bytecode);
		}
		
		putchar('\n');
	}
}

// Interpret bytecode from a source path and return an exit code.
static int interpret(const char *path) {
	uint8_t *bytecode = compilePath(path);
	
	if (bytecode != NULL) {
		int exitCode = interpretBytecode(bytecode);
		free(bytecode);
		return exitCode;
	} else {
		return EXIT_FAILURE;
	}
}

// Test Brainiac.
int main(int argc, const char *argv[]) {
	if (argc == 1) {
		return repl();
	} else if (argc == 2) {
		return interpret(argv[1]);
	} else {
		fprintf(stderr, "Usage: brainiac [path]\n");
		return EXIT_FAILURE;
	}
}
