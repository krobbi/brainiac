#include "compiler.h"
#include "generator.h"
#include "parser.h"
#include "scanner.h"

// Compile bytecode from a scanner.
static uint8_t *compileScanner(Scanner *scanner) {
	Node *ast = parseScanner(scanner);
	
	if (ast == NULL) {
		return NULL;
	}
	
	uint8_t *bytecode = generateCode(ast);
	freeNode(ast);
	return bytecode;
}

// Compile bytecode from source code.
uint8_t *compileSource(const char *source) {
	Scanner scanner;
	initScannerSource(&scanner, source);
	return compileScanner(&scanner);
}
