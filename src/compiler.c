#include <stdlib.h>

#include "compiler.h"
#ifdef BRAINIAC_DEBUG
#include "debug.h"
#endif // BRAINIAC_DEBUG
#include "generator.h"
#include "optimizer.h"
#include "parser.h"
#include "scanner.h"

// Compile bytecode from a scanner.
static uint8_t *compileScanner(Scanner *scanner) {
	Node *program = parseScanner(scanner);
	
	if (program == NULL) {
		return NULL;
	}
#ifdef BRAINIAC_DEBUG
	printf("Parsed AST:\n");
	printProgram(program);
#endif // BRAINIAC_DEBUG
	optimizeProgram(program);
	uint8_t *bytecode = compileProgram(program);
#ifdef BRAINIAC_DEBUG
	printf("\nOptimized AST:\n");
	printProgram(program);
	printf("\nCompiled bytecode:\n");
	printBytecode(bytecode);
	printf("\n");
#endif // BRAINIAC_DEBUG
	freeNode(program);
	return bytecode;
}

// Compile bytecode from a path.
uint8_t *compilePath(const char *path) {
	FILE *file = fopen(path, "rb");
	
	if (file == NULL) {
		fprintf(stderr, "Could not open '%s', file may not exist.\n", path);
		return NULL;
	}
	
	Scanner scanner;
	initScannerFile(&scanner, file);
	uint8_t *bytecode = compileScanner(&scanner);
	
	if (ferror(file) || !feof(file)) {
		fprintf(stderr, "Encountered an error while reading '%s'.\n", path);
		free(bytecode);
		bytecode = NULL;
	}
	
	fclose(file);
	return bytecode;
}

// Compile bytecode from source code.
uint8_t *compileSource(const char *source) {
	Scanner scanner;
	initScannerSource(&scanner, source);
	return compileScanner(&scanner);
}
