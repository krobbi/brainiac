#include <stdlib.h>

#include "node.h"

// Allocate a new node from its kind and value.
Node *newNode(NodeKind kind, int value) {
	Node *node = (Node*)malloc(sizeof(Node));
	
	if (node == NULL) {
		exit(EXIT_FAILURE);
	}
	
	node->kind = kind;
	node->value = value;
	node->childCount = 0;
	node->childCapacity = 0;
	node->children = NULL;
	return node;
}

// Free a node and its children.
void freeNode(Node *node) {
	for (int i = 0; i < node->childCount; i++) {
		freeNode(node->children[i]);
	}
	
	free(node->children);
	free(node);
}

// Append a child node to a parent node.
void appendNode(Node *parent, Node *child) {
	if (parent->childCount == parent->childCapacity) {
		parent->childCapacity = parent->childCapacity != 0 ? parent->childCapacity * 2 : 1;
		parent->children = (Node**)realloc(parent->children, parent->childCapacity * sizeof(Node*));
		
		if (parent->children == NULL) {
			exit(EXIT_FAILURE);
		}
	}
	
	parent->children[parent->childCount++] = child;
}

// Remove a child node from a parent node by index.
void removeNode(Node *parent, int index) {
	freeNode(parent->children[index]);
	parent->childCount--;
	
	for (int i = index; i < parent->childCount; i++) {
		parent->children[i] = parent->children[i + 1];
	}
}
