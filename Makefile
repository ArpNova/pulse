# Compiler settings
CXX = g++
CXXFLAGS = -Wall -Wextra -O2 -std=c++17 -Iinclude -static

# Directories and project files
BUILD_DIR = build
SRC_DIR = src
TARGET = $(BUILD_DIR)/pulse
SRCS = $(wildcard $(SRC_DIR)/*.cpp)
OBJS = $(patsubst $(SRC_DIR)/%.cpp, $(BUILD_DIR)/%.o, $(SRCS))

# Default rule
all: $(TARGET)

# Rule to build the actual program
$(TARGET): $(OBJS)
	mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) $^ -o $@

# Rule to build object files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Rule to run the program easily
run: $(TARGET)
	./$(TARGET)

# Rule to clean up compiled files
clean:
	rm -rf $(BUILD_DIR)

.PHONY: all run clean