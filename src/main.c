#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "opcodes.h"
#include "vm.h"

// A compiler's state.
typedef struct {
	// The number of bytes in the compiler's bytecode.
	int count;
	
	// The capacity of the compiler's bytecode.
	int capacity;
	
	// The compiler's bytecode.
	uint8_t *bytecode;
	
	// The compiler's loop depth.
	int depth;
} Compiler;

// Exit with an error message.
void error(const char *format, ...) {
	va_list args;
	va_start(args, format);
	vfprintf(stderr, format, args);
	va_end(args);
	fputc('\n', stderr);
	exit(EXIT_FAILURE);
}

// Initialize a compiler.
void compilerInit(Compiler *compiler) {
	compiler->count = 0;
	compiler->capacity = 8;
	compiler->bytecode = (uint8_t*)malloc(compiler->capacity * sizeof(uint8_t));
	compiler->depth = 0;
	
	if (compiler->bytecode == NULL) {
		error("Could not allocate bytecode.");
	}
}

// Resize a compiler's bytecode.
void compilerResize(Compiler *compiler, int capacity) {
	compiler->capacity = capacity;
	compiler->bytecode = (uint8_t*)realloc(compiler->bytecode, capacity * sizeof(uint8_t));
	
	if (compiler->bytecode == NULL) {
		error("Could not reallocate bytecode.");
	}
}

// Emit a byte of bytecode to a compiler.
void compilerEmit(Compiler *compiler, uint8_t byte) {
	if (compiler->count == compiler->capacity) {
		compilerResize(compiler, compiler->capacity * 2);
	}
	
	compiler->bytecode[compiler->count++] = byte;
}

// Parse a character with a compiler.
void compilerParse(Compiler *compiler, char character) {
	// All non-command characters are comments.
	switch (character) {
		case '>': compilerEmit(compiler, OP_RIGHT); break;
		case '<': compilerEmit(compiler, OP_LEFT); break;
		case '+': compilerEmit(compiler, OP_INCREMENT); break;
		case '-': compilerEmit(compiler, OP_DECREMENT); break;
		case '.': compilerEmit(compiler, OP_OUTPUT); break;
		case ',': compilerEmit(compiler, OP_INPUT); break;
		case '[': {
			compilerEmit(compiler, OP_BEGIN);
			compiler->depth++;
			break;
		}
		case ']': {
			if (compiler->depth > 0) {
				compilerEmit(compiler, OP_END);
				compiler->depth--;
			} else {
				error("Cannot use ']' without an opening '['.");
			}
			
			break;
		}
	}
}

// End a compiler and pass ownership of its bytecode.
uint8_t *compilerEnd(Compiler *compiler) {
	if (compiler->depth > 0) {
		error("Cannot use '[' without a closing ']'.");
	}
	
	compilerEmit(compiler, OP_HALT);
	
	if (compiler->capacity > compiler->count) {
		compilerResize(compiler, compiler->count);
	}
	
	return compiler->bytecode;
}

// Compile bytecode from a path.
uint8_t *compile(const char *path) {
	// The file to compile.
	FILE *file = fopen(path, "rb");
	
	if (file == NULL) {
		error("Could not read '%s', file may not exist.", path);
	}
	
	// The compiler to compile the file with.
	Compiler compiler;
	compilerInit(&compiler);
	
	// The character to parse.
	int character;
	
	while ((character = fgetc(file)) != EOF) {
		compilerParse(&compiler, (char)character);
	}
	
	if (ferror(file) || !feof(file)) {
		error("Could not finish reading '%s'.", path);
	}
	
	if (fclose(file) == EOF) {
		error("Could not close '%s' after reading.", path);
	}
	
	return compilerEnd(&compiler);
}

// Test Brainiac.
int main(int argc, const char *argv[]) {
	if (argc == 2) {
		uint8_t *bytecode = compile(argv[1]);
		int exitCode = interpret(bytecode);
		free(bytecode);
		return exitCode;
	} else {
		fprintf(stderr, "Usage: brainiac <path>\n");
		return EXIT_FAILURE;
	}
}
