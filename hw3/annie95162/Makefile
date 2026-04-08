PYTHON ?= python3
CXX ?= g++
PYBIND11_INCLUDES := $(shell $(PYTHON) -m pybind11 --includes)
EXT_SUFFIX := $(shell $(PYTHON)-config --extension-suffix)

CXXFLAGS := -O3 -Wall -Wextra -std=c++17 -fPIC
LDFLAGS := -shared
BLAS_LIBS := -lblas

TARGET := _matrix$(EXT_SUFFIX)
SRC := _matrix.cpp

all: $(TARGET)

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) $(PYBIND11_INCLUDES) $(SRC) $(LDFLAGS) -o $@ $(BLAS_LIBS)

test: $(TARGET)
	$(PYTHON) -m pytest -v test_matrix.py validate.py

clean:
	rm -f _matrix*.so *.o performance.txt
