#include <stdlib.h>

#include "compiler.h"
#include "generator.h"
#include "parser.h"
#include "scanner.h"

// Compile bytecode from a scanner.
static uint8_t *compileScanner(Scanner *scanner) {
	Node *program = parseScanner(scanner);
	
	if (program == NULL) {
		return NULL;
	}
	
	uint8_t *bytecode = generateCode(program);
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