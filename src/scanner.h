#ifndef BRAINIAC_SCANNER_H
#define BRAINIAC_SCANNER_H

#include <stdio.h>

// A scanner's mode.
typedef enum {
	SCANNER_MODE_FILE, // Scan from file input stream.
	SCANNER_MODE_SOURCE, // Scan from source code string.
} ScannerMode;

// A syntactic element of source code.
typedef enum {
	TK_NONE, // Comment.
	TK_EOF, // End of file.
	TK_GREATER, // '>'.
	TK_LESS, // '<'.
	TK_PLUS, // '+'.
	TK_MINUS, // '-'.
	TK_DOT, // '.'.
	TK_COMMA, // ','.
	TK_LBRACKET, // '['.
	TK_RBRACKET, // ']'.
} Token;

// Scans tokens from source code.
typedef struct {
	// The scanner's mode.
	ScannerMode mode;
	
	// The scanner's file.
	FILE *file;
	
	// The scanner's next character.
	const char *next;
} Scanner;

// Initialize a scanner from a file.
void initScannerFile(Scanner *scanner, FILE *file);

// Initialize a scanner from source code.
void initScannerSource(Scanner *scanner, const char *source);

// Scan the next token.
Token scanToken(Scanner *scanner);

#endif // BRAINIAC_SCANNER_H
