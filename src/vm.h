#ifndef brainiac_vm_h
#define brainiac_vm_h

#include <stdint.h>

// Interpret bytecode from a pointer and return an exit code.
int interpretBytecode(uint8_t *ip);

#endif // brainiac_vm_h
