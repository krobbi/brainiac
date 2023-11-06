#include <stdio.h>

#include "scanner.h"

// Dump a message from a token.
static void dumpToken(Token token) {
	switch (token) {
		case TK_NONE: printf("(None)"); break;
		case TK_EOF: printf("(EOF)"); break;
		case TK_PLUS: printf("+"); break;
		case TK_COMMA: printf(","); break;
		case TK_MINUS: printf("-"); break;
		case TK_DOT: printf("."); break;
		case TK_LESS: printf("<"); break;
		case TK_GREATER: printf(">"); break;
		case TK_LBRACKET: printf("["); break;
		case TK_RBRACKET: printf("]"); break;
	}
}

// Test the scanner.
int main() {
	Scanner scanner;
	initScanner(&scanner);
	scanFromSource(&scanner, "Comment 1 , [.,] Comment 2 < +- > Comment 3");
	Token token;
	
	do {
		token = scanToken(&scanner);
		dumpToken(token);
	} while (token != TK_EOF);
	
	printf("\n");
	return 0;
}
