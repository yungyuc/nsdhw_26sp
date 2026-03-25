# 自動偵測 pybind11 與 Python 的 Includes
# 加上 || true 是為了防止在沒裝 pybind11 的環境下 make 直接噴錯，讓 g++ 報錯更清楚
PY_INCLUDES = $(shell python3 -m pybind11 --includes 2>/dev/null || pkg-config --cflags python3)
PY_SUFFIX = $(shell python3-config --extension-suffix)

# 如果上述指令失敗，手動補上常見路徑（針對 GitHub CI 的 Ubuntu 環境）
ifeq ($(PY_INCLUDES),)
  PY_INCLUDES = -I/usr/include/python3.10 -I/usr/local/include/python3.10
endif

all: _vector$(PY_SUFFIX)

_vector$(PY_SUFFIX): _vector.cpp
	g++ -O3 -Wall -shared -fPIC $(PY_INCLUDES) _vector.cpp -o _vector$(PY_SUFFIX)

test: all
	python3 -m pytest -v test_vector.py

clean:
	rm -f *_vector$(PY_SUFFIX)
	rm -rf __pycache__ .pytest_cache