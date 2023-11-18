#ifndef BRAINIAC_SCANNER_H
#define BRAINIAC_SCANNER_H

#include <stdio.h>

// A syntactic element of source code.
typedef enum {
	TOKEN_EOF, // End of file.
	TOKEN_GREATER, // '>'.
	TOKEN_LESS, // '<'.
	TOKEN_PLUS, // '+'.
	TOKEN_MINUS, // '-'.
	TOKEN_DOT, // '.'.
	TOKEN_COMMA, // ','.
	TOKEN_LBRACKET, // '['.
	TOKEN_RBRACKET, // ']'.
} Token;

// Scans tokens from source code.
typedef struct {
	// The scanner's file.
	FILE *file;
	
	// The scanner's next character.
	const char *next;
	
	// The scanner's current line number.
	int line;
	
	// The scanner's current column number.
	int column;
	
	// The scanner's next line number.
	int nextLine;
	
	// The scanner's next column number.
	int nextColumn;
} Scanner;

// Initialize a scanner from a file.
void initScannerFile(Scanner *scanner, FILE *file);

// Initialize a scanner from source code.
void initScannerSource(Scanner *scanner, const char *source);

// Scan the next token.
Token scanToken(Scanner *scanner);

#endif // BRAINIAC_SCANNER_H
