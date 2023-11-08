#ifndef BRAINIAC_GENERATOR_H
#define BRAINIAC_GENERATOR_H

#include <stdint.h>

#include "node.h"

// Generate bytecode from an AST.
uint8_t *generateCode(Node *node);

#endif // BRAINIAC_GENERATOR_H
