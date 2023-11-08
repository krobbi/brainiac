#include <stdio.h>
#include <stdlib.h>

#include "compiler.h"
#include "opcodes.h"

// Print an operation.
static void printOp(const char *name) {
	printf("%s\n", name);
}

// Print a U8 operation.
static void printOpU8(const char *name, uint8_t *bytecode, int *index) {
	uint8_t operand = bytecode[*index];
	*index = *index + 1;
	printf("%s %d\n", name, operand);
}

// Print a U16 operation.
static void printOpU16(const char *name, uint8_t *bytecode, int *index) {
	uint8_t lowByte = bytecode[*index];
	*index = *index + 1;
	uint8_t highByte = bytecode[*index];
	*index = *index + 1;
	uint16_t operand = (uint16_t)((highByte << 8) | lowByte);
	printf("%s %d\n", name, operand);
}

// Print bytecode.
static void printBytecode(uint8_t *bytecode) {
	int index = 0;
	uint8_t opcode;
	
	do {
		printf("%04d | ", index);
		opcode = bytecode[index++];
		
		switch (opcode) {
			case OP_HLT:
				printOp("HLT");
				break;
			case OP_RGT:
				printOp("RGT");
				break;
			case OP_RGT_U8:
				printOpU8("RGT_U8", bytecode, &index);
				break;
			case OP_RGT_U16:
				printOpU16("RGT_U16", bytecode, &index);
				break;
			case OP_LFT:
				printOp("LFT");
				break;
			case OP_LFT_U8:
				printOpU8("LFT_U8", bytecode, &index);
				break;
			case OP_LFT_U16:
				printOpU16("LFT_U16", bytecode, &index);
				break;
			case OP_INC:
				printOp("INC");
				break;
			case OP_INC_U8:
				printOpU8("INC_U8", bytecode, &index);
				break;
			case OP_DEC:
				printOp("DEC");
				break;
			case OP_DEC_U8:
				printOpU8("DEC_U8", bytecode, &index);
				break;
			case OP_OUT:
				printOp("OUT");
				break;
			case OP_INP:
				printOp("INP");
				break;
			case OP_BRZ_U8:
				printOpU8("BRZ_U8", bytecode, &index);
				break;
			case OP_BRZ_U16:
				printOpU16("BRZ_U16", bytecode, &index);
				break;
			case OP_BNZ_U8:
				printOpU8("BNZ_U8", bytecode, &index);
				break;
			case OP_BNZ_U16:
				printOpU16("BNZ_U16", bytecode, &index);
				break;
			default: printf("Unknown opcode '%d'\n", opcode); break;
		}
	} while (opcode != OP_HLT);
}

// Test the compiler.
int main(int argc, const char *argv[]) {
	if (argc == 2) {
		uint8_t *bytecode = compileSource(argv[1]);
		
		if (bytecode == NULL) {
			return 1;
		}
		
		printBytecode(bytecode);
		free(bytecode);
		return 0;
	} else {
		fprintf(stderr, "Usage: brainiac <source>\n");
		return 1;
	}
}
