#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "compiler.h"
#include "opcodes.h"

#define MAX_LOOP_DEPTH 64

// A compiler's state.
typedef struct {
	// Whether the compiler has encountered an error.
	bool hasError;
	
	// The number of bytes in the compiler's bytecode.
	int count;
	
	// The compiler's bytecode capacity.
	int capacity;
	
	// The compiler's bytecode.
	uint8_t *bytecode;
	
	// The compiler's loop depth.
	int depth;
} Compiler;

// Log an error message to a compiler.
static void logError(Compiler *compiler, const char *format, ...) {
	va_list args;
	va_start(args, format);
	vfprintf(stderr, format, args);
	va_end(args);
	fputc('\n', stderr);
	compiler->hasError = true;
	
	if (compiler->bytecode != NULL) {
		free(compiler->bytecode);
		compiler->bytecode = NULL;
	}
}

// Initialize a compiler.
static void initCompiler(Compiler *compiler) {
	compiler->hasError = false;
	compiler->count = 0;
	compiler->capacity = 8;
	compiler->bytecode = (uint8_t*)malloc(compiler->capacity * sizeof(uint8_t));
	compiler->depth = 0;
	
	if (compiler->bytecode == NULL) {
		logError(compiler, "Could not allocate bytecode.");
	}
}

// Resize a compiler's bytecode capacity.
static void resizeBytecode(Compiler *compiler, int capacity) {
	compiler->capacity = capacity;
	
	if (!compiler->hasError) {
		compiler->bytecode = (uint8_t*)realloc(compiler->bytecode, capacity * sizeof(uint8_t));
		
		if (compiler->bytecode == NULL) {
			logError(compiler, "Could not reallocate bytecode.");
		}
	}
}

// Emit a byte to a compiler's bytecode.
static void emitByte(Compiler *compiler, uint8_t byte) {
	if (compiler->count >= compiler->capacity) {
		resizeBytecode(compiler, compiler->capacity * 2);
	}
	
	if (!compiler->hasError) {
		compiler->bytecode[compiler->count] = byte;
	}
	
	compiler->count++;
}

// Begin a loop.
static void beginLoop(Compiler *compiler) {
	if (compiler->depth == MAX_LOOP_DEPTH) {
		logError(compiler, "Too many nested loops.");
	}
	
	emitByte(compiler, OP_BEGIN);
	compiler->depth++;
}

// End a loop.
static void endLoop(Compiler *compiler) {
	if (compiler->depth > 0) {
		emitByte(compiler, OP_END);
		compiler->depth--;
	} else {
		logError(compiler, "Cannot use ']' without an opening '['.");
	}
}

// Compile a command.
static void compileCommand(Compiler *compiler, char command) {
	switch (command) {
		case '>': emitByte(compiler, OP_RIGHT); break;
		case '<': emitByte(compiler, OP_LEFT); break;
		case '+': emitByte(compiler, OP_INCREMENT); break;
		case '-': emitByte(compiler, OP_DECREMENT); break;
		case '.': emitByte(compiler, OP_OUTPUT); break;
		case ',': emitByte(compiler, OP_INPUT); break;
		case '[': beginLoop(compiler); break;
		case ']': endLoop(compiler); break;
	}
}

// End a compiler.
static uint8_t *endCompiler(Compiler *compiler) {
	if (compiler->depth > 0) {
		logError(compiler, "Cannot use '[' without a closing ']'.");
	}
	
	emitByte(compiler, OP_HALT);
	
	if (compiler->capacity > compiler->count) {
		resizeBytecode(compiler, compiler->count);
	}
	
	return compiler->bytecode;
}

// Compile bytecode from a path.
uint8_t *compilePath(const char *path) {
	Compiler compiler;
	initCompiler(&compiler);
	FILE *file = fopen(path, "rb");
	
	if (file == NULL) {
		logError(&compiler, "Could not open '%s', file may not exist.", path);
		return endCompiler(&compiler);
	}
	
	int command;
	
	while ((command = fgetc(file)) != EOF) {
		compileCommand(&compiler, (char)command);
	}
	
	if (ferror(file) || !feof(file)) {
		logError(&compiler, "Could not read '%s'.", path);
	}
	
	if (fclose(file) == EOF) {
		logError(&compiler, "Could not close '%s' after reading.", path);
	}
	
	return endCompiler(&compiler);
}

// Compile bytecode from source code.
uint8_t *compileSource(const char *source) {
	Compiler compiler;
	initCompiler(&compiler);
	
	while (*source) {
		compileCommand(&compiler, *source++);
	}
	
	return endCompiler(&compiler);
}

#undef MAX_LOOP_DEPTH
