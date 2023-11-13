#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#include "optimizer.h"

// Replace loop nodes that set to 0 with set nodes.
static void stepLoopToSet(Node *parent, bool *hasChanges) {
	for (int i = 0; i < parent->childCount; i++) {
		Node *loop = parent->children[i];
		stepLoopToSet(loop, hasChanges);
		
		if (loop->kind != NODE_LOOP || loop->childCount != 1) {
			continue;
		}
		
		Node *body = loop->children[0];
		uint8_t value = (uint8_t)body->value;
		
		if ((body->kind == NODE_SET && value == 0) || (body->kind == NODE_ADD && (value & 1))) {
			freeNode(loop);
			parent->children[i] = newNode(NODE_SET, 0);
			*hasChanges = true;
		}
	}
}

// Run an optimization pass and return whether any changes were made.
static bool runPass(Node *program) {
	bool hasChanges = false;
	stepLoopToSet(program, &hasChanges);
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
