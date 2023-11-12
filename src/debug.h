#ifndef BRAINIAC_DEBUG_H
#define BRAINIAC_DEBUG_H

#include <stdint.h>

#include "node.h"

// Print a program.
void printProgram(Node *program);

// Print bytecode.
void printBytecode(uint8_t *bytecode);

#endif // BRAINIAC_DEBUG_H
