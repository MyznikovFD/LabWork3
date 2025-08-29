# Compiler and flags
CXX := g++
CXXFLAGS := -std=c++17 -Werror -Wpedantic -Wall -g -fPIC -Isrc -Itests 
TEST_FLAGS := -lgtest -lgtest_main -pthread

# Directories
SRC_DIR := src
TEST_DIR := tests
BUILD_DIR := build
BIN_DIR := bin

# Targets
TEST_TARGET := $(BIN_DIR)/cyclicListTest

# Source files
SRCS := $(wildcard $(SRC_DIR)/*.cpp)
TEST_SRCS := $(wildcard $(TEST_DIR)/*.cpp)
OBJS := $(patsubst $(SRC_DIR)/%.cpp, $(BUILD_DIR)/%.o, $(SRCS))
TEST_OBJS := $(patsubst $(TEST_DIR)/%.cpp, $(BUILD_DIR)/%.o, $(TEST_SRCS))

# Dependencies
DEPS := $(wildcard $(SRC_DIR)/*.hpp)

# Default target
default: test

# Main target - test
test: $(TEST_TARGET)

# Create directories
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(BIN_DIR):
	mkdir -p $(BIN_DIR)

# Build test executable
$(TEST_TARGET): $(TEST_OBJS) $(OBJS) | $(BIN_DIR)
	$(CXX) $(TEST_OBJS) $(OBJS) $(TEST_FLAGS) -o $@

# Compile source files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp $(DEPS) | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Compile test files
$(BUILD_DIR)/%.o: $(TEST_DIR)/%.cpp $(DEPS) | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean
clean:
	rm -rf $(BUILD_DIR) $(BIN_DIR)

# Rebuild
rebuild: clean test

# Ensure directories exist before building objects
$(OBJS): | $(BUILD_DIR)
$(TEST_OBJS): | $(BUILD_DIR)

.PHONY: clean rebuild default test
