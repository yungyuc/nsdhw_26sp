cmake_minimum_required(VERSION 3.10)

# project name
project(_matrix)

# pybind11
find_package(pybind11 REQUIRED)

# add python module
pybind11_add_module(_matrix matrix.cpp)

# link MKL
# target_link_libraries(_matrix PRIVATE mkl_rt)