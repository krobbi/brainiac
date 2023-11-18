#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#include "optimizer.h"

// Return whether a node has no effect.
static bool isNodeNop(Node *node) {
	switch (node->kind) {
		case NODE_MOVE: return (int16_t)node->value == 0;
		case NODE_ADD: return (int8_t)node->value == 0;
		default: return false;
	}
}

// Merge a pair of nodes to a new single node.
static Node *mergeNodes(Node *first, Node *second) {
	switch (second->kind) {
		case NODE_LOOP:
			if (first->kind == NODE_SET && (uint8_t)first->value == 0) {
				return newNode(NODE_SET, 0);
			} else {
				return NULL;
			}
		case NODE_MOVE:
			if (first->kind == NODE_MOVE) {
				return newNode(NODE_MOVE, first->value + second->value);
			} else {
				return NULL;
			}
		case NODE_ADD:
			if (first->kind == NODE_ADD || first->kind == NODE_SET) {
				return newNode(first->kind, first->value + second->value);
			} else {
				return NULL;
			}
		case NODE_SET:
			if (first->kind == NODE_ADD || first->kind == NODE_SET) {
				return newNode(NODE_SET, second->value);
			} else {
				return NULL;
			}
		default:
			return NULL;
	}
}

// Remove nodes that have no effect.
static void stepRemoveNop(Node *parent, bool *hasChanges) {
	for (int i = parent->childCount - 1; i >= 0; i--) {
		Node *child = parent->children[i];
		stepRemoveNop(child, hasChanges);
		
		if (isNodeNop(child)) {
			removeNode(parent, i);
			*hasChanges = true;
		}
	}
}

// Replace add nodes at the start of a program with set nodes.
static void stepReplaceHeadAddSet(Node *parent, bool *hasChanges) {
	if (parent->childCount > 0 && parent->children[0]->kind == NODE_ADD) {
		parent->children[0]->kind = NODE_SET;
		*hasChanges = true;
	}
}

// Merge pairs of nodes that can be represented as a single node.
static void stepMergeNodes(Node *parent, bool *hasChanges) {
	if (parent->childCount > 0) {
		stepMergeNodes(parent->children[parent->childCount - 1], hasChanges);
	}
	
	for (int i = parent->childCount - 2; i >= 0; i--) {
		stepMergeNodes(parent->children[i], hasChanges);
		Node *merged = mergeNodes(parent->children[i], parent->children[i + 1]);
		
		if (merged != NULL) {
			removeNode(parent, i);
			freeNode(parent->children[i]);
			parent->children[i] = merged;
			*hasChanges = true;
		}
	}
}

// Replace loop nodes that set to 0 with set nodes.
static void stepReplaceLoopSet(Node *parent, bool *hasChanges) {
	for (int i = 0; i < parent->childCount; i++) {
		Node *loop = parent->children[i];
		stepReplaceLoopSet(loop, hasChanges);
		
		if (loop->kind != NODE_LOOP || loop->childCount != 1) {
			continue;
		}
		
		Node *body = loop->children[0];
		uint8_t value = (uint8_t)body->value;
		
		if ((body->kind == NODE_ADD && (value & 1)) || (body->kind == NODE_SET && value == 0)) {
			freeNode(loop);
			parent->children[i] = newNode(NODE_SET, 0);
			*hasChanges = true;
		}
	}
}

// Run an optimization pass and return whether any changes were made.
static bool runPass(Node *program) {
	bool hasChanges = false;
	stepRemoveNop(program, &hasChanges);
	stepReplaceHeadAddSet(program, &hasChanges);
	stepMergeNodes(program, &hasChanges);
	stepReplaceLoopSet(program, &hasChanges);
	return hasChanges;
}

// Optimize a program.
void optimizeProgram(Node *program) {
	bool shouldOptimize = true;
	int passCount = 0;
	
	while (shouldOptimize) {
		shouldOptimize = runPass(program);
		passCount++;
		
		if (passCount >= 256) {
			fprintf(stderr, "Possible infinite loop in optimizer.\n");
			break;
		}
	}
}
