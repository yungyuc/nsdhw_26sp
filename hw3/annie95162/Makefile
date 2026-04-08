PYTHON ?= python3
CXX ?= g++

EXT_SUFFIX := $(shell $(PYTHON)-config --extension-suffix)

PYTHON_INCLUDES := $(shell $(PYTHON)-config --includes)

PYBIND11_INCLUDES := $(shell \
	if $(PYTHON) -c "import pybind11; print(pybind11.get_include())" >/dev/null 2>&1; then \
		echo -I$$($(PYTHON) -c "import pybind11; print(pybind11.get_include())"); \
	elif [ -d /usr/include/pybind11 ]; then \
		echo -I/usr/include/pybind11; \
	elif [ -d /usr/local/include/pybind11 ]; then \
		echo -I/usr/local/include/pybind11; \
	else \
		echo ""; \
	fi)

CXXFLAGS := -O3 -Wall -Wextra -std=c++17 -fPIC
LDFLAGS := -shared
BLAS_LIBS := -lblas

TARGET := _matrix$(EXT_SUFFIX)
SRC := _matrix.cpp

all: $(TARGET)

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) $(PYTHON_INCLUDES) $(PYBIND11_INCLUDES) $(SRC) $(LDFLAGS) -o $@ $(BLAS_LIBS)

test: $(TARGET)
	PYTHONPATH=. $(PYTHON) -m pytest -v test_matrix.py
clean:
	rm -f _matrix*.so *.o performance.txt
