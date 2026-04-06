PYTHON = python3
PYBIND_INCLUDE = $(shell $(PYTHON) -m pybind11 --includes 2>/dev/null || ( $(PYTHON) -m pip install pybind11 > /dev/null && $(PYTHON) -m pybind11 --includes ) )
PYTHON_INCLUDE = $(shell $(PYTHON)-config --includes)
PYTHON_SUFFIX  = $(shell $(PYTHON)-config --extension-suffix)
CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -O2 -fPIC $(PYBIND_INCLUDE) $(PYTHON_INCLUDE)
LDFLAGS = -shared
UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S), Darwin)
    LDFLAGS += -framework Accelerate -undefined dynamic_lookup
else
	CXXFLAGS += -I/usr/include/mkl
    LDFLAGS += -lmkl_rt -lpthread -lm -ldl
endif

TARGET = _matrix$(PYTHON_SUFFIX)
SRC = matrix.cpp
OBJ = $(SRC:.cpp=.o)

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

test: $(TARGET)
	$(PYTHON) -m pytest -v

clean:
	rm -f $(TARGET) $(OBJ)
	rm -f *.so