#ifndef brainiac_opcodes_h
#define brainiac_opcodes_h

// A bytecode opcode.
enum {
	OP_HALT, // Halt execution. Equivalent to end of file.
	OP_RIGHT, // Increment the data pointer. Equivalent to '>'.
	OP_LEFT, // Decrement the data pointer. Equivalent to '<'.
	OP_INCREMENT, // Increment the pointed data. Equivalent to '+'.
	OP_DECREMENT, // Decrement the pointed data. Equivalent to '-'.
	OP_OUTPUT, // Output the pointed data as a character. Equivalent to '.'.
	OP_INPUT, // Input a character to the pointed data. Equivalent to ','.
	OP_BEGIN, // Jump forward if the pointed data is zero. Emitted from '['.
	OP_END, // Jump back if the pointed data is non-zero. Emitted from ']'.
};

#endif // brainiac_opcodes_h
