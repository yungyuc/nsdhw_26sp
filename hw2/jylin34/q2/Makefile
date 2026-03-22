CXX := g++

PYTHON_INCLUDES   := $(shell python3-config --includes)
PYBIND11_INCLUDES := $(shell python3 -m pybind11 --includes)
PYTHON_LDFLAGS    := $(shell python3-config --ldflags --embed)

INCLUDES := $(PYTHON_INCLUDES) $(PYBIND11_INCLUDES)

CXXFLAGS := -O2 -Wall -shared -std=c++14 -fPIC

MODULE_EXT := $(shell python3-config --extension-suffix)
TARGET := _vector$(MODULE_EXT)

SRCS := vector_angle.cpp

all: $(TARGET)

$(TARGET): $(SRCS)
	$(CXX) $(CXXFLAGS) $(INCLUDES) $^ -o $@ $(PYTHON_LDFLAGS)

test: all
	@echo "Running Python tests..."
	python3 -m pytest -v

clean:
	@echo "Cleaning up..."
	rm -f $(TARGET) *.o
