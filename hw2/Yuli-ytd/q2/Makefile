CXX = g++
CXXFLAGS = -O3 -Wall -shared -std=c++11 -fPIC

PYBIND_INCLUDS = $(shell python3 -m pybind11 --includes)

.PHONY: all test clean

all: _vector.so

_vector.so: _vector.cpp
	$(CXX) $(CXXFLAGS) $(PYBIND_INCLUDS) $< -o $@

test: all
	python3 -m pytest -v

clean:
	rm -rf *_vector.so __pycache__ .pytest_cache