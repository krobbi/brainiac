#include <stdio.h>
#include <stdlib.h>

#include "generator.h"
#include "opcode.h"

// A buffer of bytes.
typedef struct {
	// The number of bytes in the buffer.
	int count;
	
	// The buffer's byte capacity.
	int capacity;
	
	// The buffer's bytes.
	uint8_t *bytes;
} Buffer;

// Initialize a buffer.
static void initBuffer(Buffer *buffer) {
	buffer->count = 0;
	buffer->capacity = 0;
	buffer->bytes = NULL;
}

// Put a U8 value to a buffer.
static void putU8(Buffer *buffer, uint8_t value) {
	if (buffer->count == buffer->capacity) {
		buffer->capacity = buffer->capacity != 0 ? buffer->capacity * 2 : 8;
		buffer->bytes = (uint8_t*)realloc(buffer->bytes, buffer->capacity * sizeof(uint8_t));
		
		if (buffer->bytes == NULL) {
			exit(EXIT_FAILURE);
		}
	}
	
	buffer->bytes[buffer->count++] = value;
}

// Put a U16 value to a buffer.
static void putU16(Buffer *buffer, uint16_t value) {
	putU8(buffer, value & 0xff);
	putU8(buffer, (value >> 8) & 0xff);
}

// Generate bytecode from a node.
static void generateNodeBytecode(Buffer *buffer, Node *node);

// Generate bytecode from a program node.
static void generateProgramNodeBytecode(Buffer *buffer, Node *node) {
	for (int i = 0; i < node->childCount; i++) {
		generateNodeBytecode(buffer, node->children[i]);
	}
	
	putU8(buffer, OP_HLT);
}

// Generate bytecode from a loop node.
static void generateLoopNodeBytecode(Buffer *buffer, Node *node) {
	Buffer body;
	initBuffer(&body);
	
	for (int i = 0; i < node->childCount; i++) {
		generateNodeBytecode(&body, node->children[i]);
	}
	
	uint32_t offset = (uint32_t)(body.count + 2);
	
	if (offset > UINT8_MAX) {
		offset++;
	}
	
	if (offset > UINT16_MAX) {
		fprintf(stderr, "Could not compile loop because it contains too much code.\n");
		exit(EXIT_FAILURE);
	}
	
	if (offset <= UINT8_MAX) {
		putU8(buffer, OP_BRZ_U8);
		putU8(buffer, offset);
	} else {
		putU8(buffer, OP_BRZ_U16);
		putU16(buffer, offset);
	}
	
	for (int i = 0; i < body.count; i++) {
		putU8(buffer, body.bytes[i]);
	}
	
	free(body.bytes);
	
	if (offset <= UINT8_MAX) {
		putU8(buffer, OP_BNZ_U8);
		putU8(buffer, offset);
	} else {
		putU8(buffer, OP_BNZ_U16);
		putU16(buffer, offset);
	}
}

// Generate bytecode from a move node.
static void generateMoveNodeBytecode(Buffer *buffer, Node *node) {
	int16_t value = (int16_t)node->value;
	
	if (value == 1) {
		putU8(buffer, OP_RGT);
	} else if (value > 1 && value <= 255) {
		putU8(buffer, OP_RGT_U8);
		putU8(buffer, value);
	} else if (value > 255) {
		putU8(buffer, OP_RGT_U16);
		putU16(buffer, value);
	} else if (value == -1) {
		putU8(buffer, OP_LFT);
	} else if (value < -1 && value >= -255) {
		putU8(buffer, OP_LFT_U8);
		putU8(buffer, -value);
	} else if (value < -255) {
		putU8(buffer, OP_LFT_U16);
		putU16(buffer, -value);
	}
}

// Generate bytecode from an add node.
static void generateAddNodeBytecode(Buffer *buffer, Node *node) {
	int8_t value = (int8_t)node->value;
	
	if (value == 1) {
		putU8(buffer, OP_INC);
	} else if (value > 1) {
		putU8(buffer, OP_INC_U8);
		putU8(buffer, value);
	} else if (value == -1) {
		putU8(buffer, OP_DEC);
	} else if (value < -1) {
		putU8(buffer, OP_DEC_U8);
		putU8(buffer, -value);
	}
}

// Generate bytecode from a node.
static void generateNodeBytecode(Buffer *buffer, Node *node) {
	switch (node->kind) {
		case NODE_PROGRAM: generateProgramNodeBytecode(buffer, node); break;
		case NODE_LOOP: generateLoopNodeBytecode(buffer, node); break;
		case NODE_MOVE: generateMoveNodeBytecode(buffer, node); break;
		case NODE_ADD: generateAddNodeBytecode(buffer, node); break;
		case NODE_OUTPUT: putU8(buffer, OP_OUT); break;
		case NODE_INPUT: putU8(buffer, OP_INP); break;
	}
}

// Compile bytecode from a program.
uint8_t *compileProgram(Node *program) {
	Buffer buffer;
	initBuffer(&buffer);
	generateNodeBytecode(&buffer, program);
	return buffer.bytes;
}
