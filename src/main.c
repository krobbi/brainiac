#include <stdio.h>

#include "scanner.h"

// Dump a message from a token.
static void dumpToken(Token token) {
	switch (token) {
		case TK_NONE: printf("(None)"); break;
		case TK_EOF: printf("(EOF)"); break;
		case TK_PLUS: printf("(+)"); break;
		case TK_COMMA: printf("(,)"); break;
		case TK_MINUS: printf("(-)"); break;
		case TK_DOT: printf("(.)"); break;
		case TK_LESS: printf("(<)"); break;
		case TK_GREATER: printf("(>)"); break;
		case TK_LBRACKET: printf("([)"); break;
		case TK_RBRACKET: printf("(])"); break;
	}
}

// Test token debugging.
int main() {
	dumpToken(TK_NONE);
	dumpToken(TK_EOF);
	dumpToken(TK_PLUS);
	dumpToken(TK_COMMA);
	dumpToken(TK_MINUS);
	dumpToken(TK_DOT);
	dumpToken(TK_LESS);
	dumpToken(TK_GREATER);
	dumpToken(TK_LBRACKET);
	dumpToken(TK_RBRACKET);
	printf("\n");
	return 0;
}
