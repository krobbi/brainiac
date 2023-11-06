#include <stdio.h>

#include "scanner.h"

// Advance to the next character.
static int advance(Scanner *scanner) {
	if (scanner->next != NULL) {
		char character = *scanner->next++;
		
		if (character != '\0') {
			return (int)(unsigned char)character;
		} else {
			scanner->next = NULL;
			return EOF;
		}
	} else {
		return EOF;
	}
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

// Initialize a scanner from source code.
void initScannerSource(Scanner *scanner, const char *source) {
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
