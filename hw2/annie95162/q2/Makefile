PYTHON = python3
CXX = c++

EXT_SUFFIX = $(shell $(PYTHON) -c "import sysconfig; print(sysconfig.get_config_var('EXT_SUFFIX') or '.so')")

PYTHON_INCLUDES = $(shell $(PYTHON)-config --includes 2>/dev/null || $(PYTHON) -c "import sysconfig; print('-I' + sysconfig.get_path('include'))")

PYBIND11_INCLUDES = $(shell $(PYTHON) -c "import pybind11; print('-I' + pybind11.get_include())" 2>/dev/null || echo -I/usr/include/pybind11)

TARGET = _vector$(EXT_SUFFIX)
SRC = vector.cpp

CXXFLAGS = -O3 -Wall -shared -std=c++11 -fPIC

.PHONY: all test clean

all: $(TARGET)

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) $(PYTHON_INCLUDES) $(PYBIND11_INCLUDES) $(SRC) -o $(TARGET)

test: $(TARGET)
	PYTHONPATH=. $(PYTHON) -m pytest -v

clean:
	rm -f *.so
	rm -rf __pycache__ .pytest_cache
