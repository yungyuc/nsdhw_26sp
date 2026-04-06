cmake_minimum_required(VERSION 3.10)

# project name
project(_matrix)

# pybind11
find_package(pybind11 REQUIRED)

# add python module
pybind11_add_module(_matrix matrix.cpp)

# link MKL

# if conda -> tell where to find MKL for cmake
if(DEFINED ENV{CONDA_PREFIX})
    message(STATUS "Using MKL from Conda environment")
    target_include_directories(_matrix PRIVATE $ENV{CONDA_PREFIX}/include)
    target_link_directories(_matrix PRIVATE $ENV{CONDA_PREFIX}/lib)
endif()

# link MKL
target_link_libraries(_matrix PRIVATE mkl_rt)