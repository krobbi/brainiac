#ifndef BRAINIAC_COMPILER_H
#define BRAINIAC_COMPILER_H

#include <stdint.h>

// Compile bytecode from a path.
uint8_t *compilePath(const char *path);

// Compile bytecode from source code.
uint8_t *compileSource(const char *source);

#endif // BRAINIAC_COMPILER_H
