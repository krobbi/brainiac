# C compiler:
CC := gcc
CFLAGS := -std=c99 -Wall -Wextra -Werror -O2 -flto

# Source:
SRC := src.c

# Binary:
BIN := brainiac

# Add '.exe' extension to binary on Windows:
ifeq ($(OS),Windows_NT)
	BIN := $(BIN).exe
endif

# Make binary:
.PHONY: all
all: $(BIN)

# Clean binary:
.PHONY: clean
clean:
	@rm -f -- $(BIN)

# Compile source to binary:
$(BIN): $(SRC)
	$(CC) $(CFLAGS) $< -o $@
