#ifndef BRAINIAC_NODE_H
#define BRAINIAC_NODE_H

// A node's kind.
typedef enum {
	NODE_PROGRAM, // A program composed of a sequence of commands.
	NODE_LOOP, // A sequence of commands between a '[' and a ']' command.
	NODE_MOVE, // A sequence of '>' and '<' commands.
	NODE_ADD, // A sequence of '+' and '-' commands.
	NODE_OUTPUT, // A '.' command.
	NODE_INPUT, // A ',' command.
	NODE_SET, // Set pointed memory.
} NodeKind;

// A node of a program.
typedef struct Node {
	// The node's kind.
	NodeKind kind;
	
	// The node's value.
	int value;
	
	// The node's child count.
	int childCount;
	
	// The node's child capacity.
	int childCapacity;
	
	// The node's children.
	struct Node **children;
} Node;

// Allocate a new node from its kind and value.
Node *newNode(NodeKind kind, int value);

// Free a node and its children.
void freeNode(Node *node);

// Append a child node to a parent node.
void appendNode(Node *parent, Node *child);

// Remove a child node from a parent node by index.
void removeNode(Node *parent, int index);

#endif // BRAINIAC_NODE_H
