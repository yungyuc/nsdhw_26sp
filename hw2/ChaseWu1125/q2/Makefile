CXX = g++

PYBIND_INCLUDES = $(shell python3 -m pybind11 --includes 2>/dev/null)
PYTHON_INCLUDES = $(shell python3-config --includes)

CXXFLAGS = -std=c++11 -Wall -Wextra -O3 -shared -fPIC $(PYBIND_INCLUDES) $(PYTHON_INCLUDES)
PYTHON_SUFFIX = $(shell python3-config --extension-suffix)

TARGET = _vector$(PYTHON_SUFFIX)
SRCS = angle.cpp

.PHONY: all clean test

all: $(TARGET)
$(TARGET): $(SRCS)
	$(CXX) $(CXXFLAGS) -o $@ $^

test: $(TARGET)
	python3 -m pytest test_angle.py

clean:
	rm -rf $(TARGET) .pytest_cache __pycache__