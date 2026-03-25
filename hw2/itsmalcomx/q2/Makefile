CXX      = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -O2 -fPIC



PYTHON    ?= python3


PY_INCLUDE := $(shell $(PYTHON) -m pybind11 --includes 2>/dev/null || echo "-I/usr/include/python3.10 -I/usr/include/pybind11")
PY_SUFFIX  := $(shell $(PYTHON)-config --extension-suffix 2>/dev/null || \
               $(PYTHON) -c "import sysconfig; print(sysconfig.get_config_var('EXT_SUFFIX'))")

MODULE     = _vector$(PY_SUFFIX)

.PHONY: all test clean

all: $(MODULE)

$(MODULE): angle.cpp
	$(CXX) $(CXXFLAGS) -shared $(PY_INCLUDE) -o $@ $<

test: $(MODULE)
	env PYTHONPATH=".:$$PYTHONPATH" $(PYTHON) -m pytest -v

clean:
	rm -f $(MODULE)
	rm -rf *.so __pycache__ .pytest_cache
