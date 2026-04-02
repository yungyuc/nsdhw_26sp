#include <pybind11/pybind11.h>
#include <pybind11/operators.h>
#include <vector>
#include <stdexcept>
#include <algorithm>
#include <mkl.h>

namespace py = pybind11;

class Matrix {
public:
    Matrix(size_t nrow, size_t ncol) : m_nrow(nrow), m_ncol(ncol), m_buffer(nrow * ncol, 0.0) {}

    size_t nrow() const { return m_nrow; }
    size_t ncol() const { return m_ncol; }

    double operator()(size_t row, size_t col) const {
        return m_buffer[row * m_ncol + col];
    }
    double& operator()(size_t row, size_t col) {
        return m_buffer[row * m_ncol + col];
    }

    bool operator==(const Matrix& other) const {
        if (m_nrow != other.m_nrow || m_ncol != other.m_ncol) return false;
        return m_buffer == other.m_buffer;
    }

    const double* data() const { return m_buffer.data(); }
    double* data() { return m_buffer.data(); }

private:
    size_t m_nrow;
    size_t m_ncol;
    std::vector<double> m_buffer;
};

Matrix multiply_naive(const Matrix& mat1, const Matrix& mat2) {
    if (mat1.ncol() != mat2.nrow()) {
        throw std::invalid_argument("矩陣維度不匹配，無法相乘");
    }
    Matrix result(mat1.nrow(), mat2.ncol());
    
    for (size_t i = 0; i < mat1.nrow(); ++i) {
        for (size_t k = 0; k < mat1.ncol(); ++k) {
            for (size_t j = 0; j < mat2.ncol(); ++j) {
                result(i, j) += mat1(i, k) * mat2(k, j);
            }
        }
    }
    return result;
}

Matrix multiply_tile(const Matrix& mat1, const Matrix& mat2, size_t tsize) {
    if (mat1.ncol() != mat2.nrow()) {
        throw std::invalid_argument("矩陣維度不匹配，無法相乘");
    }
    Matrix result(mat1.nrow(), mat2.ncol());

    if (tsize == 0) return multiply_naive(mat1, mat2);

    for (size_t i0 = 0; i0 < mat1.nrow(); i0 += tsize) {
        for (size_t k0 = 0; k0 < mat1.ncol(); k0 += tsize) {
            for (size_t j0 = 0; j0 < mat2.ncol(); j0 += tsize) {
                for (size_t i = i0; i < std::min(i0 + tsize, mat1.nrow()); ++i) {
                    for (size_t k = k0; k < std::min(k0 + tsize, mat1.ncol()); ++k) {
                        for (size_t j = j0; j < std::min(j0 + tsize, mat2.ncol()); ++j) {
                            result(i, j) += mat1(i, k) * mat2(k, j);
                        }
                    }
                }

            }
        }
    }
    return result;
}

Matrix multiply_mkl(const Matrix& mat1, const Matrix& mat2) {
    if (mat1.ncol() != mat2.nrow()) {
        throw std::invalid_argument("矩陣維度不匹配，無法相乘");
    }
    Matrix result(mat1.nrow(), mat2.ncol());

    cblas_dgemm(
        CblasRowMajor,
        CblasNoTrans,
        CblasNoTrans,
        mat1.nrow(),
        mat2.ncol(),
        mat1.ncol(),
        1.0,
        mat1.data(),
        mat1.ncol(),
        mat2.data(),
        mat2.ncol(),
        0.0,
        result.data(),
        result.ncol()
    );

    return result;
}

PYBIND11_MODULE(_matrix, m) {
    m.doc() = "Matrix multiplication module";

    py::class_<Matrix>(m, "Matrix")
        .def(py::init<size_t, size_t>())
        .def_property_readonly("nrow", &Matrix::nrow)
        .def_property_readonly("ncol", &Matrix::ncol)
        .def(py::self == py::self)
        .def("__getitem__", [](const Matrix& mat, std::pair<size_t, size_t> index) {
            return mat(index.first, index.second);
        })
        .def("__setitem__", [](Matrix& mat, std::pair<size_t, size_t> index, double val) {
            mat(index.first, index.second) = val;
        });

    m.def("multiply_naive", &multiply_naive, "Naive matrix multiplication");
    m.def("multiply_tile", &multiply_tile, "Tiled matrix multiplication");
    m.def("multiply_mkl", &multiply_mkl, "MKL matrix multiplication");
}