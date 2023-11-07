#include <stdio.h>
#include <stdlib.h>

#include "node.h"

// Allocate a new AST node from its kind and value.
Node *newNode(NodeKind kind, int value) {
	Node *node = (Node*)malloc(sizeof(Node));
	
	if (node != NULL) {
		node->kind = kind;
		node->value = value;
		node->childCount = 0;
		node->childCapacity = 0;
		node->children = NULL;
		return node;
	} else {
		fprintf(stderr, "Could not allocate AST node.\n");
		return NULL;
	}
}

// Free an AST node and its children.
void freeNode(Node *node) {
	if (node != NULL) {
		if (node->children != NULL) {
			for (int i = 0; i < node->childCount; i++) {
				freeNode(node->children[i]);
			}
			
			free(node->children);
		}
		
		free(node);
	}
}

// Append a child AST node to a parent AST node.
Node *appendNode(Node *parent, Node *child) {
	if (parent == NULL || child == NULL) {
		freeNode(child);
		freeNode(parent);
		return NULL;
	}
	
	if (parent->children == NULL) {
		parent->childCapacity = 1;
		parent->children = (Node**)malloc(parent->childCapacity * sizeof(Node*));
		
		if (parent->children == NULL) {
			fprintf(stderr, "Could not allocate AST node's children.\n");
			freeNode(child);
			freeNode(parent);
			return NULL;
		}
	}
	
	if (parent->childCount == parent->childCapacity) {
		int newCapacity = parent->childCapacity * 2;
		Node **newChildren = (Node**)realloc(parent->children, newCapacity * sizeof(Node*));
		
		if (newChildren != NULL) {
			parent->childCapacity = newCapacity;
			parent->children = newChildren;
		} else {
			fprintf(stderr, "Could not reallocate AST node's children.\n");
			freeNode(child);
			freeNode(parent);
			return NULL;
		}
	}
	
	parent->children[parent->childCount++] = child;
	return parent;
}
