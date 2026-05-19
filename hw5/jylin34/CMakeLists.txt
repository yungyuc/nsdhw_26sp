cmake_minimum_required(VERSION 3.10)
project(_matrix) 

find_package(pybind11 REQUIRED)

pybind11_add_module(_matrix mm.cpp)

target_compile_options(_matrix PRIVATE -O3)

target_include_directories(_matrix PRIVATE /usr/include/mkl)

target_link_libraries(_matrix PRIVATE mkl_rt)
