#include <stdio.h>

#include "parser.h"

// Print a sequence AST node.
static void printSequence(Node *node, char negative, char positive) {
	if (node->value == 0) {
		printf("(%c/%c NOP)\n", positive, negative);
	} else if (node->value == 1) {
		printf("(%c)\n", positive);
	} else if (node->value == -1) {
		printf("(%c)\n", negative);
	} else if (node->value < 0) {
		printf("(%c * %d)\n", negative, -node->value);
	} else {
		printf("(%c * %d)\n", positive, node->value);
	}
}

// Print an AST node and its children.
static void printNode(Node *node, int depth) {
	for (int i = 0; i < depth; i++) {
		printf("| ");
	}
	
	switch (node->kind) {
		case NODE_PROGRAM: printf("({})\n"); break;
		case NODE_LOOP: printf("([])\n"); break;
		case NODE_MOVE: printSequence(node, '<', '>'); break;
		case NODE_ADD: printSequence(node, '-', '+'); break;
		case NODE_OUTPUT: printf("(.)\n"); break;
		case NODE_INPUT: printf("(,)\n"); break;
	}
	
	for (int i = 0; i < node->childCount; i++) {
		printNode(node->children[i], depth + 1);
	}
}

// Test AST nodes.
int main(int argc, const char *argv[]) {
	if (argc == 2) {
		Node *program = parseSource(argv[1]);
		
		if (program != NULL) {
			printNode(program, 0);
			freeNode(program);
		}
		
		return 0;
	} else {
		fprintf(stderr, "Usage: brainiac <source>\n");
		return 1;
	}
}
