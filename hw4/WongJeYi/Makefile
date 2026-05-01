
SRC:=$(wildcard	*.cpp)
_INSTALL_DUMMY	:=	$(shell	python3	-m	pip	install	pybind11)
PY_INCLUDES:=$(shell	python3	-m	pybind11	--includes)	
PY_EXT	:=$(shell	python3-config	--extension-suffix)	

all:$(SRC:.cpp=$(PY_EXT))	

%$(PY_EXT):	%.cpp
	g++	-O3	-Wall	-shared	-std=c++11	-fPIC	$(PY_INCLUDES)	-I/usr/include/mkl		$<	-o	$@	-L/usr/lib/x86_64-linux-gnu -lmkl_rt

.PHONY:	test	clean

test:	all
	python3	-m	unittest	test_matrix.py
clean:
	rm	-rf	*$(PY_EXT)	__pycache__	.pytest_cache

