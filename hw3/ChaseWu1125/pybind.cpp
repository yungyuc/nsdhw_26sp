#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include "matrix.hpp"

namespace py = pybind11;

PYBIND11_MODULE(_matrix, m)
{
    m.doc() = "Matrix multiplication module implemented in C++ with pybind11 bindings";
    py::class_<Matrix>(m, "Matrix")
        .def(py::init<size_t, size_t>())
        .def_property_readonly("nrow", &Matrix::rows)
        .def_property_readonly("ncol", &Matrix::cols)
        .def("__getitem__", [](const Matrix & mat, std::pair<size_t, size_t> idx) {
            return mat(idx.first, idx.second);
        })
        .def("__setitem__", [](Matrix & mat, std::pair<size_t, size_t> idx, double value) {
            mat(idx.first, idx.second) = value;
        })
        .def("__eq__", &Matrix::operator==);
    m.def("multiply_naive", &multiply_naive, "Naive matrix multiplication");
    m.def("multiply_tile", &multiply_tile, "Tiled matrix multiplication");
    m.def("multiply_mkl", &multiply_mkl, "Matrix multiplication using MKL");
}