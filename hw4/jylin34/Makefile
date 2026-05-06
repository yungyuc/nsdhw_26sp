# Makefile for matrix multiplication homework

VENV_DIR = .venv
VENV_PYTHON = ${VENV_DIR}/bin/python3

# Target name (CMake will build _matrix.cpython-*.so)
# We need to use a wildcard because CMake/pybind11 adds a suffix
TARGET_WILDCARD = _matrix*.so

.PHONY: all setup test clean performance

all: build/${TARGET_WILDCARD}

# Target to set up the Python virtual environment and install dependencies
setup:
	@echo "Setting up virtual environment and installing Python dependencies..."
	python3 -m venv ${VENV_DIR}
	${VENV_PYTHON} -m pip install --upgrade pip
	${VENV_PYTHON} -m pip install pytest numpy

# Build target using CMake
build/${TARGET_WILDCARD}: mm.cpp CMakeLists.txt
	@echo "Building C++ module with CMake..."
	mkdir -p build
	cd build && cmake .. && make
	cp build/${TARGET_WILDCARD} . # Copy the compiled module to the current directory

# Make 'test' and 'performance' depend on 'setup'
test: setup all
	@echo "Running tests..."
	${VENV_PYTHON} -m pytest test_mm.py

performance: setup all
	@echo "Running performance benchmark..."
	${VENV_PYTHON} -c "import test_mm; test_mm.run_benchmark()"

clean:
	@echo "Cleaning up..."
	rm -rf ${VENV_DIR}
	rm -rf build
	rm -f ${TARGET_WILDCARD} *.o # Remove both _matrix.so and _matrix.cpython-*.so
	rm -rf __pycache__ .pytest_cache
	rm -f performance.txt
