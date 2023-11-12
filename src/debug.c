#include <stdbool.h>
#include <stdio.h>

#include "debug.h"

// Get a node kind's name.
static const char *getNodeKindName(NodeKind kind) {
	switch (kind) {
		case NODE_PROGRAM: return "PROGRAM";
		case NODE_LOOP: return "LOOP";
		case NODE_MOVE: return "MOVE";
		case NODE_ADD: return "ADD";
		case NODE_OUTPUT: return "OUTPUT";
		case NODE_INPUT: return "INPUT";
	}
	
	return "UNKNOWN";
}

// Return whether a node kind has a value.
static bool nodeKindHasValue(NodeKind kind) {
	switch (kind) {
		case NODE_MOVE: return true;
		case NODE_ADD: return true;
		default: return false;
	}
}

// Print a node.
static void printNode(Node *node) {
	printf("(%s", getNodeKindName(node->kind));
	
	if (nodeKindHasValue(node->kind)) {
		printf(": %d", node->value);
	}
	
	printf(")\n");
}

// Print a program.
void printProgram(Node *program) {
	printNode(program);
}
