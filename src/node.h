#ifndef BRAINIAC_NODE_H
#define BRAINIAC_NODE_H

// An AST node's kind.
typedef enum {
	NODE_PROGRAM, // A program composed of a sequence of commands.
	NODE_LOOP, // A sequence of commands between a '[' and a ']' command.
	NODE_MOVE, // A sequence of '>' and '<' commands.
	NODE_ADD, // A sequence of '+' and '-' commands.
	NODE_OUTPUT, // A '.' command.
	NODE_INPUT, // A ',' command.
} NodeKind;

// A node of an AST.
typedef struct Node {
	// The AST node's kind.
	NodeKind kind;
	
	// The AST node's value.
	int value;
	
	// The AST node's child count.
	int childCount;
	
	// The AST node's child capacity.
	int childCapacity;
	
	// The AST node's children.
	struct Node **children;
} Node;

// Allocate a new AST node from its kind and value.
Node *newNode(NodeKind kind, int value);

// Append a child AST to node to a parent AST node.
void appendNode(Node *parent, Node *child);

// Free an AST node and its children.
void freeNode(Node *node);

#endif // BRAINIAC_NODE_H
