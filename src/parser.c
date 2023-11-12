#include <stdbool.h>

#include "parser.h"

// Parses a program from a scanner.
typedef struct {
	// The parser's scanner.
	Scanner *scanner;
	
	// The parser's next token.
	Token next;
} Parser;

// Initialize a parser.
static void initParser(Parser *parser, Scanner *scanner) {
	parser->scanner = scanner;
	parser->next = scanToken(scanner);
}

// Advance to the next token.
static Token advance(Parser *parser) {
	Token current = parser->next;
	parser->next = scanToken(parser->scanner);
	return current;
}

// Return whether the next token matches a token kind.
static bool match(Parser *parser, Token kind) {
	return parser->next == kind;
}

// Advance to the next token if it matches a token kind.
static bool accept(Parser *parser, Token kind) {
	if (match(parser, kind)) {
		advance(parser);
		return true;
	} else {
		return false;
	}
}

// Parse a sequence.
static Node *parseSequence(Parser *parser, NodeKind kind, Token neg, Token pos, int value) {
	while (match(parser, neg) || match(parser, pos)) {
		while (accept(parser, neg)) {
			value--;
		}
		
		while (accept(parser, pos)) {
			value++;
		}
	}
	
	return newNode(kind, value);
}

// Parse a command.
static Node *parseCommand(Parser *parser);

// Parse a loop.
static Node *parseLoop(Parser *parser) {
	bool hasError = false;
	Node *loop = newNode(NODE_LOOP, 0);
	
	while (!match(parser, TOKEN_RBRACKET) && !match(parser, TOKEN_EOF)) {
		Node *command = parseCommand(parser);
		
		if (command == NULL) {
			hasError = true;
			continue;
		}
		
		appendNode(loop, command);
	}
	
	if (!accept(parser, TOKEN_RBRACKET)) {
		fprintf(stderr, "Cannot use '[' without a matching closing ']'.\n");
		hasError = true;
	}
	
	if (hasError) {
		freeNode(loop);
		return NULL;
	}
	
	return loop;
}

// Parse a command.
static Node *parseCommand(Parser *parser) {
	switch (advance(parser)) {
		case TOKEN_EOF:
			fprintf(stderr, "Encountered end of file while parsing command.\n");
			break;
		case TOKEN_GREATER:
			return parseSequence(parser, NODE_MOVE, TOKEN_LESS, TOKEN_GREATER, 1);
		case TOKEN_LESS:
			return parseSequence(parser, NODE_MOVE, TOKEN_LESS, TOKEN_GREATER, -1);
		case TOKEN_PLUS:
			return parseSequence(parser, NODE_ADD, TOKEN_MINUS, TOKEN_PLUS, 1);
		case TOKEN_MINUS:
			return parseSequence(parser, NODE_ADD, TOKEN_MINUS, TOKEN_PLUS, -1);
		case TOKEN_DOT:
			return newNode(NODE_OUTPUT, 0);
		case TOKEN_COMMA:
			return newNode(NODE_INPUT, 0);
		case TOKEN_LBRACKET:
			return parseLoop(parser);
		case TOKEN_RBRACKET:
			fprintf(stderr, "Cannot use ']' without a matching opening '['.\n");
			break;
	}
	
	return NULL;
}

// Parse a program.
static Node *parseProgram(Parser *parser) {
	bool hasError = false;
	Node *program = newNode(NODE_PROGRAM, 0);
	
	while (!match(parser, TOKEN_EOF)) {
		Node *command = parseCommand(parser);
		
		if (command == NULL) {
			hasError = true;
			continue;
		}
		
		appendNode(program, command);
	}
	
	if (hasError) {
		freeNode(program);
		return NULL;
	}
	
	return program;
}

// Parse a program from a scanner.
Node *parseScanner(Scanner *scanner) {
	Parser parser;
	initParser(&parser, scanner);
	return parseProgram(&parser);
}
