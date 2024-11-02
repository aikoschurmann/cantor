# Directories
OUT_DIR = out
SRC_DIR = src
TEST_DIR = test
OBJ_DIR = obj

# Compiler
CC = gcc

# Project name
NAME = cantor

# Source files
SRC_FILES = $(wildcard $(SRC_DIR)/*.c)
TEST_FILES = $(wildcard $(TEST_DIR)/*.c)

# Object files
OBJ_FILES = $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SRC_FILES)) \
            $(patsubst $(TEST_DIR)/%.c,$(OBJ_DIR)/%.o,$(TEST_FILES))

# Flags
CFLAGS = -Wall -Wextra -Iinclude -g

# Targets
all: $(OUT_DIR)/$(NAME)

$(OUT_DIR)/$(NAME): $(OBJ_FILES)
	$(CC) $(OBJ_FILES) -o $@

# Compile source files to object files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR)/%.o: $(TEST_DIR)/%.c
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Clean up object files and binaries
clean:
	rm -rf $(OBJ_DIR)/* $(OUT_DIR)/*

# Run tests
run: all
	./$(OUT_DIR)/$(NAME)

.PHONY: all clean run
