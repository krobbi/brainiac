#ifndef brainiac_vm_h
#define brainiac_vm_h

#include <stdint.h>

// Interpret bytecode from a pointer and return an exit code.
int interpret(uint8_t *ip);

#endif // brainiac_vm_h
