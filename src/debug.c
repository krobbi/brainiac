#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "debug.h"

// Indent data for printing a program.
typedef struct {
	// The indent data's depth and flag count.
	int depth;
	
	// The indent data's flag capacity.
	int capacity;
	
	// The indent data's flags.
	bool *flags;
} Indent;

// Initialize indent dataa.
static void initIndent(Indent *indent) {
	indent->depth = 0;
	indent->capacity = 0;
	indent->flags = NULL;
}

// Push an indent level and flag to indent data.
static void pushIndent(Indent *indent, bool flag) {
	if (indent->depth == indent->capacity) {
		indent->capacity = indent->capacity != 0 ? indent->capacity * 2 : 8;
		indent->flags = (bool*)realloc(indent->flags, indent->capacity * sizeof(bool));
		
		if (indent->flags == NULL) {
			exit(EXIT_FAILURE);
		}
	}
	
	indent->flags[indent->depth++] = flag;
}

// Pop an indent level from indent data.
static void popIndent(Indent *indent) {
	indent->depth--;
}

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
		case NODE_MOVE:
		case NODE_ADD:
			return true;
		default:
			return false;
	}
}

// Print a node and its children.
static void printNode(Node *node, Indent *indent) {
	for (int i = 0; i < indent->depth; i++) {
		if (i == indent->depth - 1) {
			if (indent->flags[i]) {
				printf("'-");
			} else {
				printf("|-");
			}
		} else {
			if (indent->flags[i]) {
				printf("  ");
			} else {
				printf("| ");
			}
		}
	}
	
	printf("(%s", getNodeKindName(node->kind));
	
	if (nodeKindHasValue(node->kind)) {
		printf(": %d", node->value);
	}
	
	printf(")\n");
	
	for (int i = 0; i < node->childCount; i++) {
		pushIndent(indent, i == node->childCount - 1);
		printNode(node->children[i], indent);
		popIndent(indent);
	}
}

// Print a program.
void printProgram(Node *program) {
	Indent indent;
	initIndent(&indent);
	printNode(program, &indent);
	free(indent.flags);
}
