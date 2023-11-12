#include <stdio.h>

#include "opcode.h"
#include "vm.h"

// Interpret bytecode.
void interpretBytecode(uint8_t *bytecode) {
#define VM_U8() (*bytecode++)
#define VM_U16() (bytecode += 2, (uint16_t)((bytecode[-1] << 8) | bytecode[-2]))
#ifdef __GNUC__
	__label__
	vm_OP_HLT,
	vm_OP_RGT,
	vm_OP_RGT_U8,
	vm_OP_RGT_U16,
	vm_OP_LFT,
	vm_OP_LFT_U8,
	vm_OP_LFT_U16,
	vm_OP_INC,
	vm_OP_INC_U8,
	vm_OP_DEC,
	vm_OP_DEC_U8,
	vm_OP_OUT,
	vm_OP_INP,
	vm_OP_BRZ_U8,
	vm_OP_BRZ_U16,
	vm_OP_BNZ_U8,
	vm_OP_BNZ_U16;
	
	static void *dispatchTable[] = {
		[OP_HLT] = &&vm_OP_HLT,
		[OP_RGT] = &&vm_OP_RGT,
		[OP_RGT_U8] = &&vm_OP_RGT_U8,
		[OP_RGT_U16] = &&vm_OP_RGT_U16,
		[OP_LFT] = &&vm_OP_LFT,
		[OP_LFT_U8] = &&vm_OP_LFT_U8,
		[OP_LFT_U16] = &&vm_OP_LFT_U16,
		[OP_INC] = &&vm_OP_INC,
		[OP_INC_U8] = &&vm_OP_INC_U8,
		[OP_DEC] = &&vm_OP_DEC,
		[OP_DEC_U8] = &&vm_OP_DEC_U8,
		[OP_OUT] = &&vm_OP_OUT,
		[OP_INP] = &&vm_OP_INP,
		[OP_BRZ_U8] = &&vm_OP_BRZ_U8,
		[OP_BRZ_U16] = &&vm_OP_BRZ_U16,
		[OP_BNZ_U8] = &&vm_OP_BNZ_U8,
		[OP_BNZ_U16] = &&vm_OP_BNZ_U16,
	};
#define VM_OP(opcode) vm_##opcode:
#define VM_DISPATCH() goto *dispatchTable[VM_U8()]
#define VM_LOOP() VM_DISPATCH();
#else // __GNUC__
#define VM_LOOP() for (;;) switch (VM_U8())
#define VM_OP(opcode) case opcode:
#define VM_DISPATCH() break
#endif // __GNUC__
	uint16_t pointer = 0;
	uint8_t memory[UINT16_MAX + 1] = {};
	
	VM_LOOP() {
		VM_OP(OP_HLT) {
			return;
		}
		
		VM_OP(OP_RGT) {
			++pointer;
			VM_DISPATCH();
		}
		
		VM_OP(OP_RGT_U8) {
			pointer += VM_U8();
			VM_DISPATCH();
		}
		
		VM_OP(OP_RGT_U16) {
			pointer += VM_U16();
			VM_DISPATCH();
		}
		
		VM_OP(OP_LFT) {
			--pointer;
			VM_DISPATCH();
		}
		
		VM_OP(OP_LFT_U8) {
			pointer -= VM_U8();
			VM_DISPATCH();
		}
		
		VM_OP(OP_LFT_U16) {
			pointer -= VM_U16();
			VM_DISPATCH();
		}
		
		VM_OP(OP_INC) {
			++memory[pointer];
			VM_DISPATCH();
		}
		
		VM_OP(OP_INC_U8) {
			memory[pointer] += VM_U8();
			VM_DISPATCH();
		}
		
		VM_OP(OP_DEC) {
			--memory[pointer];
			VM_DISPATCH();
		}
		
		VM_OP(OP_DEC_U8) {
			memory[pointer] -= VM_U8();
			VM_DISPATCH();
		}
		
		VM_OP(OP_OUT) {
			putchar(memory[pointer]);
			VM_DISPATCH();
		}
		
		VM_OP(OP_INP) {
			int value = getchar();
			memory[pointer] = value != EOF ? (uint8_t)value : 0;
			VM_DISPATCH();
		}
		
		VM_OP(OP_BRZ_U8) {
			uint8_t offset = VM_U8();
			
			if (!memory[pointer]) {
				bytecode += offset;
			}
			
			VM_DISPATCH();
		}
		
		VM_OP(OP_BRZ_U16) {
			uint16_t offset = VM_U16();
			
			if (!memory[pointer]) {
				bytecode += offset;
			}
			
			VM_DISPATCH();
		}
		
		VM_OP(OP_BNZ_U8) {
			uint8_t offset = VM_U8();
			
			if (memory[pointer]) {
				bytecode -= offset;
			}
			
			VM_DISPATCH();
		}
		
		VM_OP(OP_BNZ_U16) {
			uint16_t offset = VM_U16();
			
			if (memory[pointer]) {
				bytecode -= offset;
			}
			
			VM_DISPATCH();
		}
	}
#undef VM_LOOP
#undef VM_DISPATCH
#undef VM_OP
#undef VM_U16
#undef VM_U8
}
