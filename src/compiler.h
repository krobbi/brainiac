#ifndef brainiac_compiler_h
#define brainiac_compiler_h

#include <stdint.h>

// Compile bytecode from a path.
uint8_t *compilePath(const char *path);

// Compile bytecode from source code.
uint8_t *compileSource(const char *source);

#endif // brainiac_compiler_h
