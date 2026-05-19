#include <cstddef>
#include <algorithm>
#include <stdexcept>
#include <vector>
#include <cmath>
#include <mkl_cblas.h>

#include "matrix.hpp"

// --- 記憶體監控全域變數 ---
static size_t total_allocated_bytes = 0;
static size_t total_deallocated_bytes = 0;

// --- 自定義分配器 實作 ---
template <class T>
T* CustomAllocator<T>::allocate(std::size_t n) {
    if (n > std::size_t(-1) / sizeof(T)) throw std::bad_alloc();
    if (auto p = static_cast<T*>(std::malloc(n * sizeof(T)))) {
        total_allocated_bytes += n * sizeof(T);
        return p;
    }
    throw std::bad_alloc();
}

template <class T>
void CustomAllocator<T>::deallocate(T* p, std::size_t n) noexcept {
    total_deallocated_bytes += n * sizeof(T);
    std::free(p);
}

template <class T, class U>
bool operator==(const CustomAllocator<T>&, const CustomAllocator<U>&) { return true; }
template <class T, class U>
bool operator!=(const CustomAllocator<T>&, const CustomAllocator<U>&) { return false; }

// --- 提供給 Python 的監控函式 ---
size_t get_bytes() { return total_allocated_bytes - total_deallocated_bytes; }
size_t get_allocated() { return total_allocated_bytes; }
size_t get_deallocated() { return total_deallocated_bytes; }

// --- Matrix 方法實作 ---
Matrix::Matrix(size_t nrow, size_t ncol)
  : m_nrow(nrow), m_ncol(ncol), m_buffer(nrow * ncol, 0.0)
{}

Matrix::~Matrix() = default;
Matrix::Matrix(const Matrix& other) = default;
Matrix& Matrix::operator=(const Matrix& other) = default;
Matrix::Matrix(Matrix&& other) noexcept = default;
Matrix& Matrix::operator=(Matrix&& other) noexcept = default;

size_t Matrix::nrow() const { return m_nrow; }
size_t Matrix::ncol() const { return m_ncol; }

double& Matrix::operator()(size_t i, size_t j) {
    if (i >= m_nrow || j >= m_ncol) throw std::out_of_range("Matrix access out of range");
    return m_buffer[i * m_ncol + j];
}

double Matrix::operator()(size_t i, size_t j) const {
    if (i >= m_nrow || j >= m_ncol) throw std::out_of_range("Matrix access out of range");
    return m_buffer[i * m_ncol + j];
}

double* Matrix::get_buffer() { return m_buffer.data(); }
const double* Matrix::get_buffer() const { return m_buffer.data(); }

bool Matrix::operator==(const Matrix& other) const {
    if (m_nrow != other.m_nrow || m_ncol != other.m_ncol) return false;
    for (size_t i = 0; i < m_nrow * m_ncol; i++) {
        if (std::abs(m_buffer[i] - other.m_buffer[i]) > 1e-9) return false;
    }
    return true;
}

// --- 乘法實作 ---
Matrix multiply_naive(Matrix const &mat1, Matrix const &mat2){
    if(mat1.ncol() != mat2.nrow()){ throw std::runtime_error("Incompatible matrix dimensions"); }
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
    if(mat1.ncol() != mat2.nrow()){ throw std::runtime_error("Incompatible matrix dimensions"); }
    Matrix ret(mat1.nrow(), mat2.ncol());
    cblas_dgemm(
        CblasRowMajor, CblasNoTrans, CblasNoTrans,
        mat1.nrow(), mat2.ncol(), mat1.ncol(), 1.0,
        mat1.get_buffer(), mat1.ncol(),
        mat2.get_buffer(), mat2.ncol(),
        0.0, ret.get_buffer(), ret.ncol()
    );
    return ret;
}

Matrix multiply_tile(const Matrix& A, const Matrix& B, size_t tile_size) {
    if (A.ncol() != B.nrow()) { throw std::invalid_argument("Matrix dimensions are not compatible for multiplication"); }
    Matrix C(A.nrow(), B.ncol());
    for (size_t i0 = 0; i0 < A.nrow(); i0 += tile_size) {
        for (size_t k0 = 0; k0 < A.ncol(); k0 += tile_size) {
            for (size_t j0 = 0; j0 < B.ncol(); j0 += tile_size) {
                for (size_t i = i0; i < std::min(i0 + tile_size, A.nrow()); ++i) {
                    for (size_t k = k0; k < std::min(k0 + tile_size, A.ncol()); ++k) {
                        double r = A(i, k);
                        for (size_t j = j0; j < std::min(j0 + tile_size, B.ncol()); ++j) {
                            C(i, j) += r * B(k, j);
                        }
                    }
                }
            }
        }
    }
    return C;
}

