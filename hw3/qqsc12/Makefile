CXX = g++
# -O3 和 -mavx2 是效能達標的關鍵
CXXFLAGS = -O3 -march=native -Wall -shared -std=c++14 -fPIC $(shell python3 -m pybind11 --include)
# 修正後的 MKL 編譯參數
MKL_INCLUDES = -I/usr/include/mkl
MKL_LDFLAGS = -lmkl_intel_lp64 -lmkl_sequential -lmkl_core -lpthread -lm -ldl

TARGET = _matrix$(shell python3-config --extension-suffix)

.PHONY: all clean test

all: $(TARGET)

$(TARGET): _matrix.cpp
	$(CXX) $(CXXFLAGS) $(MKL_INCLUDES) $< -o $@ $(MKL_LDFLAGS)

clean:
	rm -rf $(TARGET) __pycache__ .pytest_cache performance.txt

test: all
	python3 -m pytest test_matrix.py