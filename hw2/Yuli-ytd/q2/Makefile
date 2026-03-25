CXX = g++
CXXFLAGS = -O3 -Wall -shared -std=c++11 -fPIC

PYTHON_INCLUDES := $(shell python3-config --includes)
PYBIND11_INCLUDES := $(shell python3 -m pybind11 --includes 2>/dev/null)
INCLUDES := $(PYTHON_INCLUDES) $(PYBIND11_INCLUDES)

.PHONY: all test clean

all: _vector.so

_vector.so: _vector.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) $< -o $@

test: all
	python3 -m pytest -v

clean:
	rm -rf *_vector.so __pycache__ .pytest_cache