cmake_minimum_required(VERSION 3.10)

# project name
project(_matrix)

# pybind11
find_package(pybind11 REQUIRED)

# add python module
pybind11_add_module(_matrix matrix.cpp)

# compile options
target_compile_options(_matrix PRIVATE -O3)

# link MKL

# if conda -> tell where to find MKL for cmake
if(DEFINED ENV{CONDA_PREFIX})
    message(STATUS "Using MKL from Conda environment")
    target_include_directories(_matrix PRIVATE $ENV{CONDA_PREFIX}/include)
    target_link_directories(_matrix PRIVATE $ENV{CONDA_PREFIX}/lib)
else()
    # CI apt install intel-mkl-full -> headers in /usr/include/mkl
    target_include_directories(_matrix PRIVATE /usr/include/mkl)
endif()

# link MKL
target_link_libraries(_matrix PRIVATE mkl_rt)