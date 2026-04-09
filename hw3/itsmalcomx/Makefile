CXX      := g++
PYTHON   := python3


PY_INC   := $(shell $(PYTHON) -c "import pybind11; print(pybind11.get_include())" 2>/dev/null || echo "/usr/include/pybind11")
PY_FLAGS := $(shell $(PYTHON)-config --includes --ldflags)


ifneq ($(or $(PRELOAD_MKL),$(wildcard PRELOAD_MKL)),)
  BLAS_INC  := /usr/include/mkl
  BLAS_LIB  := -Wl,--no-as-needed -lmkl_rt -Wl,--as-needed
else
  OPENBLAS_REAL := $(shell readlink -f /usr/lib/x86_64-linux-gnu/libopenblas.so 2>/dev/null)
  OPENBLAS_DIR  := $(shell dirname $(OPENBLAS_REAL))
  BLAS_INC  := /usr/include/x86_64-linux-gnu
  BLAS_LIB  := -L$(OPENBLAS_DIR) -Wl,--no-as-needed -lopenblas -Wl,--as-needed \
               -Wl,-rpath,$(OPENBLAS_DIR)
endif

EXT_SUFFIX := $(shell $(PYTHON)-config --extension-suffix)
TARGET     := _matrix$(EXT_SUFFIX)

CXXFLAGS := -O3 -march=native -std=c++17 -fPIC \
            -I$(PY_INC) -I$(BLAS_INC)
LDFLAGS  := $(BLAS_LIB) -shared

.PHONY: all test clean

all: $(TARGET)

$(TARGET): matrix.cpp
	$(CXX) $(CXXFLAGS) $(PY_FLAGS) $(LDFLAGS) -o $@ $<

test: $(TARGET)
	$(PYTHON) -m pytest test_matrix.py -v

clean:
	rm -f $(TARGET) performance.txt
	rm -rf __pycache__ .pytest_cache
