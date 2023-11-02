# C compiler:
CC := gcc
CFLAGS := -std=c99 -Wall -Wextra -Werror -O2 -flto

# Directories:
SRC_DIR := src
BIN_DIR := bin

# Files:
SRCS := $(wildcard $(SRC_DIR)/*.c)
HDRS := $(wildcard $(SRC_DIR)/*.h)
OBJS := $(SRCS:$(SRC_DIR)/%.c=$(BIN_DIR)/%.o)
EXEC := $(BIN_DIR)/brainiac
TEST := test/cat.bf

# Add '.exe' extension to executable on Windows:
ifeq ($(OS),Windows_NT)
	EXEC := $(EXEC).exe
endif

# Build Brainiac:
.PHONY: all
all: $(EXEC)

# Clean binaries directory:
.PHONY: clean
clean:
	@echo "Cleaning '$(BIN_DIR)'..."
	@rm -rf -- $(BIN_DIR)

# Test Brainiac:
.PHONY: test
test: $(EXEC)
	@echo "Running '$(TEST)'..."
	@$(EXEC) $(TEST)

# Make binaries directory:
$(BIN_DIR):
	@echo "Making '$@'..."
	@mkdir $(BIN_DIR)

# Compile object from source:
$(BIN_DIR)/%.o: $(SRC_DIR)/%.c $(HDRS) | $(BIN_DIR)
	@echo "Compiling '$<'..."
	@$(CC) $(CFLAGS) -c $< -o $@

# Link executable from objects:
$(EXEC): $(OBJS) | $(BIN_DIR)
	@echo "Linking '$@'..."
	@$(CC) $(CFLAGS) $^ -o $@
