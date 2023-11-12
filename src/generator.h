#ifndef BRAINIAC_GENERATOR_H
#define BRAINIAC_GENERATOR_H

#include <stdint.h>

#include "node.h"

// Compile bytecode from a program.
uint8_t *compileProgram(Node *program);

#endif // BRAINIAC_GENERATOR_H
