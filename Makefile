# C compiler:
CC := gcc
CFLAGS := -std=c99 -Wall -Wextra -Werror

# Directories:
SRC_DIR := src
BIN_DIR := bin

# Files:
SRCS := $(wildcard $(SRC_DIR)/*.c)
HDRS := $(wildcard $(SRC_DIR)/*.h)
OBJS := $(SRCS:$(SRC_DIR)/%.c=$(BIN_DIR)/%.o)
EXEC := $(BIN_DIR)/brainiac

# Add '.exe' extension to executable on Windows:
ifeq ($(OS),Windows_NT)
	EXEC := $(EXEC).exe
endif

# Build Brainiac in release mode:
.PHONY: all
all: CFLAGS += -O2 -flto
all: $(EXEC)

# Build Brainiac in debug mode:
.PHONY: debug
debug: CFLAGS += -DBRAINIAC_DEBUG -g3 -Og
debug: $(EXEC)

# Clean binaries directory:
.PHONY: clean
clean:
	@echo "Cleaning '$(BIN_DIR)'..."
	@rm -rf -- $(BIN_DIR)

# Make binaries directory:
$(BIN_DIR):
	@echo "Making '$@'..."
	@mkdir $(BIN_DIR)

# Compile object from source:
$(BIN_DIR)/%.o: $(SRC_DIR)/%.c $(HDRS) | $(BIN_DIR)
	@echo "Compiling '$<'..."
	@$(CC) $(CFLAGS) -c $< -o $@

# Link executable from objects:
$(EXEC): $(OBJS)
	@echo "Linking '$@'..."
	@$(CC) $(CFLAGS) $^ -o $@
