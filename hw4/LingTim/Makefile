CXX = g++
CXXFLAGS = -O3 -Wall -shared -std=c++11 -fPIC $(shell python3-config --includes) -I/usr/include/mkl
# 連結 MKL 的函式庫 (根據系統環境可能需要微調，此為常見設定)
LDFLAGS = -Wl,--no-as-needed -lmkl_intel_lp64 -lmkl_sequential -lmkl_core -lpthread -lm -ldl

# 自動獲取 Python 對應的 C++ extension 後綴 (例如 .cpython-310-x86_64-linux-gnu.so)
TARGET = _matrix$(shell python3-config --extension-suffix)

.PHONY: all clean

all: $(TARGET)

$(TARGET): matrix.cpp
	$(CXX) $(CXXFLAGS) $< -o $@ $(LDFLAGS)

clean:
	rm -f *.so *.o