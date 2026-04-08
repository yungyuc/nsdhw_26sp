# 1. 偵測 Python 路徑
PYTHON_INC    := $(shell python3-config --includes)
PY_EXT_SUFFIX := $(shell python3-config --extension-suffix)

# 2. 指定 pybind11 路徑 (相容 CI 與本地)
PYBIND11_INC  := -I/usr/include -I/usr/local/include -I/usr/lib/python3/dist-packages/pybind11/include

# 3. 編譯參數
CXX = g++
CXXFLAGS = -O3 -march=native -Wall -shared -std=c++14 -fPIC \
           $(PYTHON_INC) $(PYBIND11_INC) \
           -I/usr/include/mkl

# 4. 關鍵修正：連結 mkl_rt 並強制加入 RPATH 
# 這能讓 loader 在執行時優先去正確的地方找 mkl 庫
LDFLAGS = -lmkl_rt -lpthread -lm -ldl -Wl,-rpath,/usr/lib/x86_64-linux-gnu

# 5. 目標
TARGET = _matrix$(PY_EXT_SUFFIX)

all: $(TARGET)

$(TARGET): _matrix.cpp
	$(CXX) $(CXXFLAGS) _matrix.cpp -o $(TARGET) $(LDFLAGS)

clean:
	rm -rf $(TARGET) __pycache__ .pytest_cache performance.txt

test: $(TARGET)
	PYTHONPATH=. python3 -m pytest test_matrix.py