#include <stdbool.h>
#include <stdio.h>

#include "parser.h"

// Parses an AST from a scanner.
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
	
	while (!match(parser, TK_RBRACKET) && !match(parser, TK_EOF)) {
		Node *command = parseCommand(parser);
		
		if (command == NULL) {
			hasError = true;
			continue;
		}
		
		appendNode(loop, command);
	}
	
	if (!accept(parser, TK_RBRACKET)) {
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
		case TK_NONE:
			fprintf(stderr, "Bug: Encountered comment token while parsing command.\n");
			break;
		case TK_EOF:
			fprintf(stderr, "Bug: Encountered end of file token while parsing command.\n");
			break;
		case TK_GREATER:
			return parseSequence(parser, NODE_MOVE, TK_LESS, TK_GREATER, 1);
		case TK_LESS:
			return parseSequence(parser, NODE_MOVE, TK_LESS, TK_GREATER, -1);
		case TK_PLUS:
			return parseSequence(parser, NODE_ADD, TK_MINUS, TK_PLUS, 1);
		case TK_MINUS:
			return parseSequence(parser, NODE_ADD, TK_MINUS, TK_PLUS, -1);
		case TK_DOT:
			return newNode(NODE_OUTPUT, 0);
		case TK_COMMA:
			return newNode(NODE_INPUT, 0);
		case TK_LBRACKET:
			return parseLoop(parser);
		case TK_RBRACKET:
			fprintf(stderr, "Cannot use ']' without a matching opening '['.\n");
			break;
	}
	
	return NULL;
}

// Parse a program.
static Node *parseProgram(Parser *parser) {
	bool hasError = false;
	Node *program = newNode(NODE_PROGRAM, 0);
	
	while (!match(parser, TK_EOF)) {
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
