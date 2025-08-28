CXX := g++
CXXFLAGS := -std=c++17 -Werror -Wpedantic -Wall -g -fPIC -Isrc 


SRC_DIR := src
BUILD_DIR := build
TARGET := $(BUILD_DIR)/cyclicList

DEPS = $(wildcard $(SRC_DIR)/*.hpp)
SRCS = $(wildcard $(SRC_DIR)/*.cpp)
OBJS = $(patsubst $(SRC_DIR)/%.cpp, $(BUILD_DIR)/%.o, $(SRCS))

default: all

all: $(TARGET)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(TARGET): $(OBJS) | $(BUILD_DIR)
	$(CXX) $(OBJS) -o $@

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp $(DEPS) | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -rf $(BUILD_DIR)

rebuild: clean all

.PHONY: all clean rebuild default
