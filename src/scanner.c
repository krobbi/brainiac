#include "scanner.h"

// Advance to the next character.
static int advance(Scanner *scanner) {
	if (scanner->file != NULL) {
		return fgetc(scanner->file);
	}
	
	char current = *scanner->next++;
	
	if (current == '\0') {
		return EOF;
	}
	
	return (int)(unsigned char)current;
}

// Initialize a scanner from a file.
void initScannerFile(Scanner *scanner, FILE *file) {
	scanner->file = file;
	scanner->next = NULL;
}

// Initialize a scanner from source code.
void initScannerSource(Scanner *scanner, const char *source) {
	scanner->file = NULL;
	scanner->next = source;
}

// Scan the next token.
Token scanToken(Scanner *scanner) {
	for (;;) {
		switch (advance(scanner)) {
			case EOF: return TOKEN_EOF;
			case '+': return TOKEN_PLUS;
			case ',': return TOKEN_COMMA;
			case '-': return TOKEN_MINUS;
			case '.': return TOKEN_DOT;
			case '<': return TOKEN_LESS;
			case '>': return TOKEN_GREATER;
			case '[': return TOKEN_LBRACKET;
			case ']': return TOKEN_RBRACKET;
		}
	}
}
