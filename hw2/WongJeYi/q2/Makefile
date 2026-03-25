SRC:=$(wildcard	*.cpp)
PYTHON_BIN:=	python3
PY_INCLUDES	:=	$(shell	$(PYTHON_BIN)	-m	pybind11	--includes)	
PY_CONFIG	:=	$(shell	python3-config	--includes)
PY_EXT	:=	$(shell	python3-config	--extension-suffix)	

all:$(SRC:.cpp=$(PY_EXT))	

%$(PY_EXT):	%.cpp
	$(PYTHON_BIN)	-m	pip	install	--user	pybind11
	$(eval	PY_INCLUDES=$(shell	$(PYTHON_BIN)	-m	pybind11	--includes))
	g++	-O3	-Wall	-shared	-std=c++11	-fPIC	$(PY_CONFIG)	$(PY_INCLUDES)	$<	-o	$@

.PHONY:	test

test:
	$(PYTHON_BIN)	-m	pip	install	--user	pytest
	$(PYTHON_BIN)	-m	pytest	-v	-s
clean:
	rm	-f	*$(PY_EXT)
	rm	-rf	__pycache__
	rm	-rf	.pytest_cache
	rm	-f	$(SRC:.cpp=$(PY_EXT))
