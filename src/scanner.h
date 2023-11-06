#ifndef BRAINIAC_SCANNER_H
#define BRAINIAC_SCANNER_H

// A syntactic element of source code.
typedef enum {
	TK_NONE, // Non-token.
	TK_EOF, // End of file.
	TK_PLUS, // '+'.
	TK_COMMA, // ','.
	TK_MINUS, // '-'.
	TK_DOT, // '.'.
	TK_LESS, // '<'.
	TK_GREATER, // '>'.
	TK_LBRACKET, // '['.
	TK_RBRACKET, // ']'.
} Token;

#endif // BRAINIAC_SCANNER_H
