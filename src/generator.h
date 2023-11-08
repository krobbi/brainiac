#ifndef BRAINIAC_GENERATOR_H
#define BRAINIAC_GENERATOR_H

#include <stdint.h>

#include "node.h"

// Generate bytecode from a program.
uint8_t *generateCode(Node *program);

#endif // BRAINIAC_GENERATOR_H
