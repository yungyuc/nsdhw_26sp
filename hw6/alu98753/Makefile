PYTHON = python3
PYBIND_INCLUDE = $(shell $(PYTHON) -m pybind11 --includes)
PYTHON_INCLUDE = $(shell $(PYTHON)-config --includes)
NUMPY_INCLUDE = $(shell $(PYTHON) -c "import numpy; print('-I' + numpy.get_include())")
PYTHON_SUFFIX  = $(shell $(PYTHON)-config --extension-suffix)

CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -O3 -fPIC $(PYBIND_INCLUDE) $(PYTHON_INCLUDE) $(NUMPY_INCLUDE) -I/usr/include/mkl
LDFLAGS = -shared -lmkl_rt -lpthread -lm -ldl

TARGET = _matrix$(PYTHON_SUFFIX)
SRC = matrix.cpp mod.cpp
OBJ = $(SRC:.cpp=.o)

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(TARGET) $(OBJ)
	rm -f *.so
