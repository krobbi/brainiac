#include <stdio.h>
#include <stdlib.h>

#include "opcodes.h"
#include "vm.h"

// Interpret bytecode from a pointer and return an exit code.
int interpretBytecode(uint8_t *ip) {
#ifdef __GNUC__
	__label__
	vm_OP_HALT,
	vm_OP_RIGHT,
	vm_OP_LEFT,
	vm_OP_INCREMENT,
	vm_OP_DECREMENT,
	vm_OP_OUTPUT,
	vm_OP_INPUT,
	vm_OP_BEGIN,
	vm_OP_END;
	
	static void *dispatchTable[] = {
		[OP_HALT] = &&vm_OP_HALT,
		[OP_RIGHT] = &&vm_OP_RIGHT,
		[OP_LEFT] = &&vm_OP_LEFT,
		[OP_INCREMENT] = &&vm_OP_INCREMENT,
		[OP_DECREMENT] = &&vm_OP_DECREMENT,
		[OP_OUTPUT] = &&vm_OP_OUTPUT,
		[OP_INPUT] = &&vm_OP_INPUT,
		[OP_BEGIN] = &&vm_OP_BEGIN,
		[OP_END] = &&vm_OP_END,
	};
#define VM_OP(opcode) vm_##opcode:
#define VM_DISPATCH goto *dispatchTable[*ip++]
#define VM_LOOP VM_DISPATCH;
#else // __GNUC__
#define VM_LOOP for(;;) switch(*ip++)
#define VM_OP(opcode) case opcode:
#define VM_DISPATCH break
#endif // __GNUC__
#define VM_WORD (ip += 2, ip[-1] << 8 | ip[-2])
	uint16_t mp = 0;
	uint8_t *memory = (uint8_t*)calloc(UINT16_MAX + 1, sizeof(uint8_t));
	
	if (memory == NULL) {
		fprintf(stderr, "Could not allocate VM memory.\n");
		return EXIT_FAILURE;
	}
	
	VM_LOOP {
		VM_OP(OP_HALT) {
			free(memory);
			return EXIT_SUCCESS;
		}
		
		VM_OP(OP_RIGHT) {
			++mp;
			VM_DISPATCH;
		}
		
		VM_OP(OP_LEFT) {
			--mp;
			VM_DISPATCH;
		}
		
		VM_OP(OP_INCREMENT) {
			++memory[mp];
			VM_DISPATCH;
		}
		
		VM_OP(OP_DECREMENT) {
			--memory[mp];
			VM_DISPATCH;
		}
		
		VM_OP(OP_OUTPUT) {
			putchar(memory[mp]);
			VM_DISPATCH;
		}
		
		VM_OP(OP_INPUT) {
			int character = getchar();
			memory[mp] = character != EOF ? (uint8_t)character : 0;
			VM_DISPATCH;
		}
		
		VM_OP(OP_BEGIN) {
			int offset = VM_WORD;
			
			if (!memory[mp]) {
				ip += offset;
			}
			
			VM_DISPATCH;
		}
		
		VM_OP(OP_END) {
			int offset = VM_WORD;
			
			if (memory[mp]) {
				ip -= offset;
			}
			
			VM_DISPATCH;
		}
	}
#undef VM_LOOP
#undef VM_OP
#undef VM_DISPATCH
#undef VM_WORD
}
