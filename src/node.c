#include <stdio.h>
#include <stdlib.h>

#include "node.h"

// Allocate a new AST node from its kind and value.
Node *newNode(NodeKind kind, int value) {
	Node *node = (Node*)malloc(sizeof(Node));
	
	if (node == NULL) {
		fprintf(stderr, "Could not allocate AST node.\n");
		return NULL;
	}
	
	node->kind = kind;
	node->value = value;
	node->childCount = 0;
	node->childCapacity = 0;
	node->children = NULL;
	return node;
}

// Free an AST node and its children.
void freeNode(Node *node) {
	for (int i = 0; i < node->childCount; i++) {
		freeNode(node->children[i]);
	}
	
	if (node->childCapacity > 0) {
		free(node->children);
	}
	
	free(node);
}

// Append a child AST node to a parent AST node.
void appendNode(Node *parent, Node *child) {
	if (parent->childCapacity == 0) {
		parent->childCapacity = 1;
		parent->children = (Node**)malloc(parent->childCapacity * sizeof(Node*));
		
		if (parent->children == NULL) {
			fprintf(stderr, "Could not allocate AST node's children.\n");
			parent->childCapacity = 0;
			freeNode(child);
			return;
		}
	}
	
	if (parent->childCount >= parent->childCapacity) {
		int newCapacity = parent->childCapacity * 2;
		Node **newChildren = (Node**)realloc(parent->children, newCapacity * sizeof(Node*));
		
		if (newChildren == NULL) {
			fprintf(stderr, "Could not reallocate AST node's children.\n");
			freeNode(child);
			return;
		}
		
		parent->childCapacity = newCapacity;
		parent->children = newChildren;
	}
	
	parent->children[parent->childCount++] = child;
}
