#include <cstddef>
#include <algorithm>
#include <pybind11/pybind11.h>
#include <pybind11/operators.h>
#include <mkl_cblas.h>
namespace py = pybind11;

class Matrix {
    private:
        size_t m_nrow;
        size_t m_ncol;
        double* m_buffer; // 1D array, len =nrow * ncol

    public:
        // Constructor
        Matrix(size_t nrow, size_t ncol){
            m_nrow = nrow;
            m_ncol = ncol;
            m_buffer = new double[nrow * ncol];
            // init all to 0
            std::fill(m_buffer, m_buffer + nrow * ncol, 0.0);
        } 
        
        // Destructor
        ~Matrix() {
            delete[] m_buffer;
        }

        // 存取
        size_t nrow() const { return m_nrow; }
        size_t ncol() const { return m_ncol; }
        double& operator()(size_t i, size_t j) { return m_buffer[i * m_ncol + j]; }
        double operator()(size_t i, size_t j) const { return m_buffer[i * m_ncol + j]; }
        double* get_buffer() const { return m_buffer; }
        // compare
        /*        // === 比較 ===
        operator==(other) -> bool:
            如果大小不同 回傳 false
            逐一比較每個元素
            全部相同 回傳 true*/
        bool operator==(const Matrix& other) const {
            if(m_nrow != other.m_nrow || m_ncol != other.m_ncol) return false;
            for(size_t i = 0; i< m_nrow* m_ncol; i++){
                if(m_buffer[i] != other.m_buffer[i]) return false;
            }
            return true;
        }

        // copy constructor
        Matrix(const Matrix& other) : m_nrow(other.m_nrow), m_ncol(other.m_ncol){
            m_buffer = new double[m_nrow * m_ncol];
            std::copy(other.m_buffer, other.m_buffer + m_nrow * m_ncol, m_buffer);
        }

        // copy assignment
        Matrix& operator=(const Matrix& other){
            if(this == &other) return *this;
            if(m_nrow != other.m_nrow || m_ncol != other.m_ncol){
                delete[] m_buffer;
                m_nrow = other.m_nrow;
                m_ncol = other.m_ncol;
                m_buffer = new double[m_nrow * m_ncol];
            }
            std::copy(other.m_buffer, other.m_buffer + m_nrow * m_ncol, m_buffer);
            return *this;
        }

        // move constructor
        Matrix(Matrix&& other) noexcept : m_nrow(other.m_nrow), m_ncol(other.m_ncol), m_buffer(other.m_buffer){
            other.m_nrow = 0;
            other.m_ncol = 0;
            other.m_buffer = nullptr;
        }

        // move assignment
        Matrix& operator=(Matrix&& other) noexcept{
            if(this == &other) return *this;
            delete[] m_buffer;
            m_nrow = other.m_nrow;
            m_ncol = other.m_ncol;
            m_buffer = other.m_buffer;
            other.m_nrow = 0;
            other.m_ncol = 0;
            other.m_buffer = nullptr;
            return *this;
        }
};

// 1. Naive multiplication
Matrix multiply_naive(Matrix const &mat1, Matrix const &mat2){
    if(mat1.ncol() != mat2.nrow()){
        throw std::runtime_error("Incompatible matrix dimensions");
    }
    Matrix C(mat1.nrow(),mat2.ncol());
    for(size_t i=0; i<mat1.nrow(); i++){
        for(size_t j=0; j<mat2.ncol();j++){
            double sum = 0.0;
            for(size_t k=0; k<mat1.ncol();k++){
                sum += mat1(i,k) * mat2(k,j);
            }
            C(i,j) = sum;
        }
    }
    return C;
}

Matrix multiply_mkl(Matrix const &mat1, Matrix const &mat2){
    Matrix ret(mat1.nrow(), mat2.ncol());
    if(mat1.ncol() != mat2.nrow()){
        throw std::runtime_error("Incompatible matrix dimensions");
    }
    cblas_dgemm(
        CblasRowMajor, CblasNoTrans, CblasNoTrans, 
        mat1.nrow(), mat2.ncol(), mat1.ncol(), 1.0, 
        mat1.get_buffer(), mat1.ncol(), 
        mat2.get_buffer(), mat2.ncol(), 
        0.0, ret.get_buffer(), ret.ncol()
    );
    return ret;
}

PYBIND11_MODULE(_matrix, m){
    m.doc() = "Matrix class for homework 3";
    py::class_<Matrix>(m, "Matrix")
        .def(py::init<size_t, size_t>())
        .def_property_readonly("nrow", &Matrix::nrow)
        .def_property_readonly("ncol", &Matrix::ncol)
        .def("__getitem__", [](Matrix& m, py::tuple idx){
            return m(idx[0].cast<size_t>(), idx[1].cast<size_t>());
        })
        .def("__setitem__", [](Matrix& m, py::tuple idx, double val){
            m(idx[0].cast<size_t>(), idx[1].cast<size_t>()) = val;
        })
        .def("__eq__", &Matrix::operator==);
    m.def("multiply_naive", &multiply_naive);
    m.def("multiply_mkl", &multiply_mkl);
    // m.def("multiply_tile", &multiply_tile);
}