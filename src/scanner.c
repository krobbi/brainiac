#include "scanner.h"

// Advance to the next character in file mode.
static int advanceFile(Scanner *scanner) {
	return fgetc(scanner->file);
}

// Advance to the next character in source mode.
static int advanceSource(Scanner *scanner) {
	if (*scanner->next != '\0') {
		return (int)(unsigned char)*scanner->next++;
	} else {
		return EOF;
	}
}

// Advance to the next character.
static int advance(Scanner *scanner) {
	switch (scanner->mode) {
		case SCANNER_MODE_FILE: return advanceFile(scanner);
		case SCANNER_MODE_SOURCE: return advanceSource(scanner);
	}
	
	return EOF;
}

// Scan the next token or comment.
static Token scanRawToken(Scanner *scanner) {
	int character = advance(scanner);
	
	if (character != EOF) {
		switch ((char)(unsigned char)character) {
			case '+': return TK_PLUS;
			case ',': return TK_COMMA;
			case '-': return TK_MINUS;
			case '.': return TK_DOT;
			case '<': return TK_LESS;
			case '>': return TK_GREATER;
			case '[': return TK_LBRACKET;
			case ']': return TK_RBRACKET;
			default: return TK_NONE;
		}
	} else {
		return TK_EOF;
	}
}

// Initialize a scanner from a file.
void initScannerFile(Scanner *scanner, FILE *file) {
	scanner->mode = SCANNER_MODE_FILE;
	scanner->file = file;
	scanner->next = NULL;
}

// Initialize a scanner from source code.
void initScannerSource(Scanner *scanner, const char *source) {
	scanner->mode = SCANNER_MODE_SOURCE;
	scanner->file = NULL;
	scanner->next = source;
}

// Scan the next token.
Token scanToken(Scanner *scanner) {
	Token token;
	
	do {
		token = scanRawToken(scanner);
	} while (token == TK_NONE);
	
	return token;
}
