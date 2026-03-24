CXX = g++
CXXFLAGS = -std=c++17 -O2 -Wall -shared -fPIC

PYTHON_INCLUDES = $(shell python3 -m pybind11 --includes)
PYTHON_SUFFIX = $(shell python3-config --extension-suffix)
TARGET = _vector$(PYTHON_SUFFIX)

all: $(TARGET)

$(TARGET): angle.cpp
	$(CXX) $(CXXFLAGS) $(PYTHON_INCLUDES) -o $@ $<

test: $(TARGET)
	python3 -m pytest -v

clean:
	rm -f $(TARGET) *.so
	rm -rf __pycache__ .pytest_cache

.PHONY: all test clean
