#include <stdbool.h>
#include <stdio.h>

#include "parser.h"
#include "scanner.h"

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

// Log an error message and return a null node.
static Node *error(const char *message) {
	fprintf(stderr, "%s\n", message);
	return NULL;
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
	Node *loop = newNode(NODE_LOOP, 0);
	
	while (!match(parser, TK_RBRACKET) && !match(parser, TK_EOF)) {
		Node *command = parseCommand(parser);
		
		if (command != NULL) {
			appendNode(loop, command);
		}
	}
	
	if (accept(parser, TK_RBRACKET)) {
		return loop;
	} else {
		freeNode(loop);
		return error("Cannot use '[' without a matching closing ']'.");
	}
}

// Parse a command.
static Node *parseCommand(Parser *parser) {
	Token kind = advance(parser);
	
	switch (kind) {
		case TK_GREATER: return parseSequence(parser, NODE_MOVE, TK_LESS, TK_GREATER, 1);
		case TK_LESS: return parseSequence(parser, NODE_MOVE, TK_LESS, TK_GREATER, -1);
		case TK_PLUS: return parseSequence(parser, NODE_ADD, TK_MINUS, TK_PLUS, 1);
		case TK_MINUS: return parseSequence(parser, NODE_ADD, TK_MINUS, TK_PLUS, -1);
		case TK_DOT: return newNode(NODE_OUTPUT, 0);
		case TK_COMMA: return newNode(NODE_INPUT, 0);
		case TK_LBRACKET: return parseLoop(parser);
		case TK_RBRACKET: return error("Cannot use ']' without a matching opening '['.");
		default: return error("Bug: Unhandled token kind in command parser.");
	}
}

// Parse a program.
static Node *parseProgram(Parser *parser) {
	Node *program = newNode(NODE_PROGRAM, 0);
	
	while (!match(parser, TK_EOF)) {
		Node *command = parseCommand(parser);
		
		if (command != NULL) {
			appendNode(program, command);
		}
	}
	
	return program;
}

// Parse a program from source code.
Node *parseSource(const char *source) {
	Scanner scanner;
	Parser parser;
	initScannerSource(&scanner, source);
	initParser(&parser, &scanner);
	return parseProgram(&parser);
}
