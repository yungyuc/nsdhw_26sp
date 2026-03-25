# 自動取得編譯參數
PY_INCLUDES = $(shell python3 -m pybind11 --includes)
PY_SUFFIX = $(shell python3-config --extension-suffix)

all: _vector$(PY_SUFFIX)

_vector$(PY_SUFFIX): _vector.cpp
	g++ -O3 -Wall -shared -fPIC $(PY_INCLUDES) _vector.cpp -o _vector$(PY_SUFFIX)

# 老師要求的測試指令
test: all
	python3 -m pytest -v test_vector.py

clean:
	rm -f *_vector$(PY_SUFFIX)
	rm -rf __pycache__ .pytest_cache