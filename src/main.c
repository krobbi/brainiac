#include <stdio.h>
#include <stdlib.h>

#include "generator.h"
#include "opcodes.h"
#include "parser.h"

// Print a sequence AST node.
static void printSequence(Node *node, char negative, char positive) {
	if (node->value == 0) {
		printf("(%c/%c NOP)\n", positive, negative);
	} else if (node->value == 1) {
		printf("(%c)\n", positive);
	} else if (node->value == -1) {
		printf("(%c)\n", negative);
	} else if (node->value < 0) {
		printf("(%c * %d)\n", negative, -node->value);
	} else {
		printf("(%c * %d)\n", positive, node->value);
	}
}

// Print an AST node and its children.
static void printNode(Node *node, int depth) {
	for (int i = 0; i < depth; i++) {
		printf("| ");
	}
	
	switch (node->kind) {
		case NODE_PROGRAM: printf("({})\n"); break;
		case NODE_LOOP: printf("([])\n"); break;
		case NODE_MOVE: printSequence(node, '<', '>'); break;
		case NODE_ADD: printSequence(node, '-', '+'); break;
		case NODE_OUTPUT: printf("(.)\n"); break;
		case NODE_INPUT: printf("(,)\n"); break;
	}
	
	for (int i = 0; i < node->childCount; i++) {
		printNode(node->children[i], depth + 1);
	}
}

// Print an operation.
static void printOp(const char *name) {
	printf("%s\n", name);
}

// Print a U8 operation.
static void printOpU8(const char *name, uint8_t *bytecode, int *index) {
	uint8_t operand = bytecode[*index];
	*index = *index + 1;
	printf("%s %d ; U8 operand.\n", name, operand);
}

// Print a U16 operation.
static void printOpU16(const char *name, uint8_t *bytecode, int *index) {
	uint8_t lowByte = bytecode[*index];
	*index = *index + 1;
	uint8_t highByte = bytecode[*index];
	*index = *index + 1;
	uint16_t operand = (uint16_t)((highByte << 8) | lowByte);
	printf("%s %d ; U16 operand.\n", name, operand);
}

// Print bytecode.
static void printBytecode(uint8_t *bytecode) {
	int index = 0;
	uint8_t opcode;
	
	do {
		printf("%04d | ", index);
		opcode = bytecode[index++];
		
		switch (opcode) {
			case OP_HLT: printOp("HLT"); break;
			case OP_RGT: printOp("RGT"); break;
			case OP_RGT_U8: printOpU8("RGT", bytecode, &index); break;
			case OP_RGT_U16: printOpU16("RGT", bytecode, &index); break;
			case OP_LFT: printOp("LFT"); break;
			case OP_LFT_U8: printOpU8("LFT", bytecode, &index); break;
			case OP_LFT_U16: printOpU16("LFT", bytecode, &index); break;
			case OP_INC: printOp("INC"); break;
			case OP_INC_U8: printOpU8("INC", bytecode, &index); break;
			case OP_DEC: printOp("DEC"); break;
			case OP_DEC_U8: printOpU8("DEC", bytecode, &index); break;
			case OP_OUT: printOp("OUT"); break;
			case OP_INP: printOp("INP"); break;
			case OP_BRZ_U8: printOpU8("BRZ", bytecode, &index); break;
			case OP_BRZ_U16: printOpU16("BRZ", bytecode, &index); break;
			case OP_BNZ_U8: printOpU8("BNZ", bytecode, &index); break;
			case OP_BNZ_U16: printOpU16("BNZ", bytecode, &index); break;
			default: printf("Unknown opcode '%d'\n", opcode); break;
		}
	} while (opcode != OP_HLT);
}

// Test AST nodes.
int main(int argc, const char *argv[]) {
	if (argc == 2) {
		Node *ast = parseSource(argv[1]);
		
		if (ast != NULL) {
			printNode(ast, 0);
			printf("\n");
			uint8_t *bytecode = generateCode(ast);
			freeNode(ast);
			printBytecode(bytecode);
			free(bytecode);
		}
		
		return 0;
	} else {
		fprintf(stderr, "Usage: brainiac <source>\n");
		return 1;
	}
}
