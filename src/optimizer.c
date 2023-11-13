#include <stdbool.h>
#include <stdio.h>

#include "optimizer.h"

// Run an optimization pass and return whether any changes were made.
static bool runPass(Node *program) {
	(void)program; // Unused parameter.
	
	bool hasChanges = false;
	// TODO: Implement optimization steps.
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
