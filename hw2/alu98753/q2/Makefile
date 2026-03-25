CXX = g++
CXXFLAGS = -std=c++17 -O2 -Wall -shared -fPIC

PYBIND_INCLUDES = $(shell python3 -m pybind11 --includes 2>/dev/null)
PYTHON_INCLUDES = $(shell python3-config --includes 2>/dev/null)
PYTHON_SUFFIX = $(shell python3-config --extension-suffix)
TARGET = _vector$(PYTHON_SUFFIX)

all: $(TARGET)

$(TARGET): angle.cpp
	$(CXX) $(CXXFLAGS) $(PYBIND_INCLUDES) $(PYTHON_INCLUDES) -o $@ $<

test: $(TARGET)
	python3 -m pytest -v

clean:
	rm -f $(TARGET) *.so
	rm -rf __pycache__ .pytest_cache

.PHONY: all test clean
