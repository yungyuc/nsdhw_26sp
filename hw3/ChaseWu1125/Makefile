CXX = g++
CXXFLAGS = -O3 -std=c++11 -Wall -shared -fPIC

PYTHON_INCLUDES = $(shell python3-config --includes)
PYBIND_INCLUDES = $(shell python3 -m pybind11 --includes 2>/dev/null || echo "-I/usr/include/pybind11 -I/usr/local/include/pybind11")

EXTRA_INCLUDES = -I/usr/include/mkl -I/usr/include/mkl/intel64
MKL_LIBS = -lmkl_rt

SUFFIX = $(shell python3-config --extension-suffix)
TARGET = _matrix$(SUFFIX)

.PHONY: all clean test

all: $(TARGET)

$(TARGET): matrix.cpp pybind.cpp
	$(CXX) $(CXXFLAGS) $(PYTHON_INCLUDES) $(PYBIND_INCLUDES) $(EXTRA_INCLUDES) matrix.cpp pybind.cpp -o $(TARGET) $(MKL_LIBS)

test: $(TARGET)
	python3 -m pytest test_matrix.py -v

clean:
	rm -f *.so result.txt performance.txt
	rm -rf __pycache__ .pytest_cache
