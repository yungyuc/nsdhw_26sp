PYTHON = python3
CXX = c++

PYBIND11_INCLUDES = $(shell $(PYTHON) -m pybind11 --includes)
EXT_SUFFIX = $(shell $(PYTHON) -c "import sysconfig; print(sysconfig.get_config_var('EXT_SUFFIX') or '.so')")

TARGET = _vector$(EXT_SUFFIX)
SRC = vector.cpp

CXXFLAGS = -O3 -Wall -shared -std=c++11 -fPIC

.PHONY: all test clean

all: $(TARGET)

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) $(PYBIND11_INCLUDES) $(SRC) -o $(TARGET)

test: $(TARGET)
	PYTHONPATH=. $(PYTHON) -m pytest -v

clean:
	rm -f *.so
	rm -rf __pycache__ .pytest_cache
