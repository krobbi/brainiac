#ifndef BRAINIAC_OPCODE_H
#define BRAINIAC_OPCODE_H

// A bytecode opcode.
typedef enum {
	OP_HLT, // Halt execution.
	
	OP_RGT, // Increment memory pointer.
	OP_RGT_U8, // Increment memory pointer by U8 operand.
	OP_RGT_U16, // Increment memory pointer by U16 operand.
	
	OP_LFT, // Decrement memory pointer.
	OP_LFT_U8, // Decrement memory pointer by U8 operand.
	OP_LFT_U16, // Decrement memory pointer by U16 operand.
	
	OP_INC, // Increment pointed memory.
	OP_INC_U8, // Increment pointed memory by U8 operand.
	
	OP_DEC, // Decrement pointed memory.
	OP_DEC_U8, // Decrement pointed memory by U8 operand.
	
	OP_OUT, // Output from pointed memory.
	
	OP_INP, // Input to pointed memory.
	
	OP_BRZ_U8, // Branch forward by U8 operand if pointed memory is zero.
	OP_BRZ_U16, // Branch forward by U16 operand if pointed memory is zero.
	
	OP_BNZ_U8, // Branch backward by U8 operand if pointed memory is non-zero.
	OP_BNZ_U16, // Branch backward by U16 operand if pointed memory is non-zero.
	
	OP_SET_0, // Set pointed memory to 0.
	OP_SET_1, // Set pointed memory to 1.
	OP_SET_U8, // Set pointed memory to U8 operand.
} Opcode;

#endif // BRAINIAC_OPCODE_H
