CXX = g++
CXXFLAGS = -std=c++11 -Wall -Wextra -O3 -shared -fPIC $(shell python3 -m pybind11 --includes)
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