#ifndef BRAINIAC_COMPILER_H
#define BRAINIAC_COMPILER_H

#include <stdint.h>

// Compile bytecode from source code.
uint8_t *compileSource(const char *source);

#endif // BRAINIAC_COMPILER_H
