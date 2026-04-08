CXX = g++
CXXFLAGS = -O3 -Wall -std=c++11 -fPIC
LDFLAGS = -lmkl_rt

PYTHON_INCLUDES := $(shell python3-config --includes)
PYBIND11_INCLUDES := $(shell python3 -m pybind11 --includes 2>/dev/null)
MKL_INCLUDES := -I/usr/include/mkl
INCLUDES := $(PYTHON_INCLUDES) $(PYBIND11_INCLUDES) $(MKL_INCLUDES)

.PHONY = all test clean

all: _matrix.so

_matrix.so: matrix_matrix.cpp Matrix.hpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -shared $< -o $@ $(LDFLAGS)

test: all
	python3 -m pytest -v

clean:
	rm -rf *.so __pycache__ .pytest_cache