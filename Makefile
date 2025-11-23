CXX      = g++
CXXFLAGS = -O3 -std=c++17

SRCS = src/main.cpp \
       src/logistic.cpp \
       src/composition.cpp \
       src/plotter.cpp

OUT_DIR = output
TARGET  = $(OUT_DIR)/run_orbit

all: $(TARGET)

$(TARGET): $(SRCS)
	@mkdir -p $(OUT_DIR)
	@echo "Compiling and Linking..."
	$(CXX) $(CXXFLAGS) $(SRCS) -o $(TARGET)
	@echo "Build successful! Executable is at: $(TARGET)"

run: all
	@echo "Running program..."
	./$(TARGET)

clean:
	@echo "Cleaning up..."
	rm -rf $(OUT_DIR)

.PHONY: all clean run