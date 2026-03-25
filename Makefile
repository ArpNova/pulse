# Compiler settings
CXX = g++
CXXFLAGS = -Wall -Wextra -O2 -std=c++17

# Directories and project files
BUILD_DIR = build
TARGET = $(BUILD_DIR)/pulse
SRC = src/monitor.cpp

# Default rule (what happens when you just type 'make')
all: $(TARGET)

# Rule to build the actual program
$(TARGET): $(SRC)
	mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) $(SRC) -o $(TARGET)

# Rule to run the program easily
run: $(TARGET)
	./$(TARGET)

# Rule to clean up compiled files
clean:
	rm -rf $(BUILD_DIR)

.PHONY: all run clean