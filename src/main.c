#include <stdio.h>

#include "node.h"

// Print an AST node and its children.
static void printNode(Node *node, int depth) {
	for (int i = 0; i < depth; i++) {
		printf(" *");
	}
	
	if (depth > 0) {
		printf(" ");
	}
	
	switch (node->kind) {
		case NODE_PROGRAM: printf("({})\n"); break;
		case NODE_LOOP: printf("([])\n"); break;
		case NODE_MOVE: printf("(> * %d)\n", node->value); break;
		case NODE_ADD: printf("(+ * %d)\n", node->value); break;
		case NODE_OUTPUT: printf("(.)\n"); break;
		case NODE_INPUT: printf("(,)\n"); break;
	}
	
	for (int i = 0; i < node->childCount; i++) {
		printNode(node->children[i], depth + 1);
	}
}

// Create a loop for testing AST nodes.
static Node *createTestLoop() {
	Node *loop = newNode(NODE_LOOP, 0);
	appendNode(loop, newNode(NODE_OUTPUT, 0));
	appendNode(loop, newNode(NODE_INPUT, 0));
	return loop;
}

// Test AST nodes.
int main() {
	Node *program = newNode(NODE_PROGRAM, 0);
	appendNode(program, newNode(NODE_INPUT, 0));
	appendNode(program, createTestLoop());
	printNode(program, 0);
	freeNode(program);
	return 0;
}
