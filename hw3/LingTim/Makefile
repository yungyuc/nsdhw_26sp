CXX = g++

PYTHON_INCLUDES = $(shell python3-config --includes)
PYBIND11_INCLUDES = $(shell python3 -m pybind11 --includes 2>/dev/null || echo "")

MKL_INCLUDES = -I/usr/include/mkl

CXXFLAGS = -O3 -Wall -shared -std=c++11 -fPIC $(PYTHON_INCLUDES) $(PYBIND11_INCLUDES) $(MKL_INCLUDES)
PYTHON_SUFFIX = $(shell python3-config --extension-suffix)

TARGET = _matrix$(PYTHON_SUFFIX)

MKL_LIBS = -L/usr/lib/x86_64-linux-gnu -Wl,--no-as-needed -lmkl_intel_lp64 -lmkl_sequential -lmkl_core -lpthread -lm -ldl

.PHONY: all clean test

all: $(TARGET)

$(TARGET): matrix.cpp
	$(CXX) $(CXXFLAGS) matrix.cpp -o $(TARGET) $(MKL_LIBS)

test: $(TARGET)
	python3 -m pytest test_matrix.py -v

clean:
	rm -rf $(TARGET) __pycache__ .pytest_cache