# Compiler
CXX = g++

# Compiler flags
CXXFLAGS = -lpthread

# Target executable
TARGET = setAndRun.exe

# Source files
SRCS = setControlModeAndRun.cpp

# Default target
all: $(TARGET)

# Rule to build the target
$(TARGET): $(SRCS)
	$(CXX) -o $(TARGET) $(SRCS) $(CXXFLAGS)

# Clean up build files
clean:
	rm -f $(TARGET)
