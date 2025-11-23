CXX      = g++
CXXFLAGS = -O3 -std=c++17 -I./lib

APP_SRC  = src/main.cpp

LIB_SRCS = lib/logistic.cpp \
           lib/composition.cpp \
           lib/plotter.cpp \
           lib/animator.cpp

SRCS = $(APP_SRC) $(LIB_SRCS)

OUT_DIR = output
TARGET  = $(OUT_DIR)/run_orbit

all: $(TARGET)

$(TARGET): $(SRCS)
	@mkdir -p $(OUT_DIR)
	@echo "Compiling Project..."
	$(CXX) $(CXXFLAGS) $(SRCS) -o $(TARGET)
	@echo "Done! Executable: $(TARGET)"

run: all
	@echo "Running..."
	./$(TARGET)

clean:
	rm -rf $(OUT_DIR)

.PHONY: all clean run