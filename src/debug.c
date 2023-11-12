#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "debug.h"
#include "opcode.h"

// Indent data for printing a program.
typedef struct {
	// The indent data's depth and flag count.
	int depth;
	
	// The indent data's flag capacity.
	int capacity;
	
	// The indent data's flags.
	bool *flags;
} Indent;

// Initialize indent dataa.
static void initIndent(Indent *indent) {
	indent->depth = 0;
	indent->capacity = 0;
	indent->flags = NULL;
}

// Push an indent level and flag to indent data.
static void pushIndent(Indent *indent, bool flag) {
	if (indent->depth == indent->capacity) {
		indent->capacity = indent->capacity != 0 ? indent->capacity * 2 : 8;
		indent->flags = (bool*)realloc(indent->flags, indent->capacity * sizeof(bool));
		
		if (indent->flags == NULL) {
			exit(EXIT_FAILURE);
		}
	}
	
	indent->flags[indent->depth++] = flag;
}

// Pop an indent level from indent data.
static void popIndent(Indent *indent) {
	indent->depth--;
}

// Get a node kind's name.
static const char *getNodeKindName(NodeKind kind) {
	switch (kind) {
		case NODE_PROGRAM: return "PROGRAM";
		case NODE_LOOP: return "LOOP";
		case NODE_MOVE: return "MOVE";
		case NODE_ADD: return "ADD";
		case NODE_OUTPUT: return "OUTPUT";
		case NODE_INPUT: return "INPUT";
	}
	
	return "UNKNOWN";
}

// Return whether a node kind has a value.
static bool nodeKindHasValue(NodeKind kind) {
	switch (kind) {
		case NODE_MOVE:
		case NODE_ADD:
			return true;
		default:
			return false;
	}
}

// Print a node and its children.
static void printNode(Node *node, Indent *indent) {
	for (int i = 0; i < indent->depth; i++) {
		if (i == indent->depth - 1) {
			if (indent->flags[i]) {
				printf("'-");
			} else {
				printf("|-");
			}
		} else {
			if (indent->flags[i]) {
				printf("  ");
			} else {
				printf("| ");
			}
		}
	}
	
	printf("(%s", getNodeKindName(node->kind));
	
	if (nodeKindHasValue(node->kind)) {
		printf(": %d", node->value);
	}
	
	printf(")\n");
	
	for (int i = 0; i < node->childCount; i++) {
		pushIndent(indent, i == node->childCount - 1);
		printNode(node->children[i], indent);
		popIndent(indent);
	}
}

// Print a program.
void printProgram(Node *program) {
	Indent indent;
	initIndent(&indent);
	printNode(program, &indent);
	free(indent.flags);
}

// Fetch a U8 value from bytecode.
static uint8_t fetchU8(uint8_t *bytecode, int *offset) {
	return bytecode[(*offset)++];
}

// Fetch a U16 value from bytecode.
static uint16_t fetchU16(uint8_t *bytecode, int *offset) {
	uint8_t lowByte = fetchU8(bytecode, offset);
	uint8_t highByte = fetchU8(bytecode, offset);
	return (uint16_t)((highByte << 8) | lowByte);
}

// Print a basic operation.
static void printBasicOp(const char *name) {
	printf("%s\n", name);
}

// Print a U8 operation.
static void printU8Op(const char *name, uint8_t *bytecode, int *offset) {
	printf("%s %d\n", name, fetchU8(bytecode, offset));
}

// Print a U16 operation.
static void printU16Op(const char *name, uint8_t *bytecode, int *offset) {
	printf("%s %d\n", name, fetchU16(bytecode, offset));
}

// Print a jump operation.
static void printJumpOp(const char *name, uint8_t *bytecode, int *offset, int size, int sign) {
	int32_t jump = (int32_t)(size == 16 ? fetchU16(bytecode, offset) : fetchU8(bytecode, offset));
	printf("%s %d ; -> @%04d\n", name, jump, *offset + sign * jump);
}

// Print bytecode.
void printBytecode(uint8_t *bytecode) {
	int offset = 0;
	Opcode opcode;
	
	do {
		printf("@%04d | ", offset);
		opcode = (Opcode)fetchU8(bytecode, &offset);
		
		switch (opcode) {
			case OP_HLT: printBasicOp("HLT"); break;
			case OP_RGT: printBasicOp("RGT"); break;
			case OP_RGT_U8: printU8Op("RGT_U8", bytecode, &offset); break;
			case OP_RGT_U16: printU16Op("RGT_U16", bytecode, &offset); break;
			case OP_LFT: printBasicOp("LFT"); break;
			case OP_LFT_U8: printU8Op("LFT_U8", bytecode, &offset); break;
			case OP_LFT_U16: printU16Op("LFT_U16", bytecode, &offset); break;
			case OP_INC: printBasicOp("INC"); break;
			case OP_INC_U8: printU8Op("INC_U8", bytecode, &offset); break;
			case OP_DEC: printBasicOp("DEC"); break;
			case OP_DEC_U8: printU8Op("DEC_U8", bytecode, &offset); break;
			case OP_OUT: printBasicOp("OUT"); break;
			case OP_INP: printBasicOp("INP"); break;
			case OP_BRZ_U8: printJumpOp("BRZ_U8", bytecode, &offset, 8, 1); break;
			case OP_BRZ_U16: printJumpOp("BRZ_U16", bytecode, &offset, 16, 1); break;
			case OP_BNZ_U8: printJumpOp("BNZ_U8", bytecode, &offset, 8, -1); break;
			case OP_BNZ_U16: printJumpOp("BNZ_U16", bytecode, &offset, 16, -1); break;
		}
	} while (opcode != OP_HLT);
}
