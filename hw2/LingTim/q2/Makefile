CXX = g++
PYBIND_INCLUDES = `python3 -m pybind11 --includes`
EXTENSION = `python3-config --extension-suffix`

CXXFLAGS = -O3 -Wall -shared -std=c++11 -fPIC $(PYBIND_INCLUDES)
TARGET = _vector$(EXTENSION)

all: $(TARGET)

$(TARGET): _vector.cpp
	$(CXX) $(CXXFLAGS) $< -o $@

test: $(TARGET)
	env PYTHONPATH=".:$$PYTHONPATH" python3 -m pytest -v

clean:
	rm -f *.so *.o
	rm -rf .pytest_cache
	rm -rf __pycache__

.PHONY: all test clean