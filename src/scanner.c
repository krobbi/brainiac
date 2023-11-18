#include "scanner.h"

// Initialize a scanner's position.
static void initScannerPosition(Scanner *scanner) {
	scanner->line = 1;
	scanner->column = 1;
	scanner->nextLine = 1;
	scanner->nextColumn = 1;
}

// Initialize a scanner from a file.
void initScannerFile(Scanner *scanner, FILE *file) {
	scanner->file = file;
	scanner->next = NULL;
	initScannerPosition(scanner);
}

// Initialize a scanner from source code.
void initScannerSource(Scanner *scanner, const char *source) {
	scanner->file = NULL;
	scanner->next = source;
	initScannerPosition(scanner);
}

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

// Scan the next token.
Token scanToken(Scanner *scanner) {
	for (;;) {
		scanner->line = scanner->nextLine;
		scanner->column = scanner->nextColumn++;
		
		switch (advance(scanner)) {
			case EOF:
				return TOKEN_EOF;
			case '\t':
				scanner->nextColumn += 3 - ((scanner->nextColumn - 2) & 3);
				break;
			case '\n':
				scanner->nextLine++;
				scanner->nextColumn = 1;
				break;
			case '+':
				return TOKEN_PLUS;
			case ',':
				return TOKEN_COMMA;
			case '-':
				return TOKEN_MINUS;
			case '.':
				return TOKEN_DOT;
			case '<':
				return TOKEN_LESS;
			case '>':
				return TOKEN_GREATER;
			case '[':
				return TOKEN_LBRACKET;
			case ']':
				return TOKEN_RBRACKET;
		}
	}
}
