# 1. Compiler settings
CXX = g++
CXXFLAGS = -O3 -std=c++17

# 2. List your source files here
SRCS = src/main.cpp src/logistic.cpp src/composition.cpp src/plotter.cpp

# 3. Name your output file
TARGET = output

# 4. The build rule (Standard Magic)
all:
	$(CXX) $(CXXFLAGS) $(SRCS) -o $(TARGET)

clean:
	rm -f $(TARGET)