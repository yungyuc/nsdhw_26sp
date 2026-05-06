#include <pybind11/pybind11.h>
#include <pybind11/operators.h>
#include <vector>
#include <stdexcept>
#include <algorithm>
#include <mkl.h>
#include <atomic>
#include <cstdlib>

namespace py = pybind11;

// ==============================================================================
// 1. 實作全域的記憶體追蹤計數器與 Custom STL Allocator
// ==============================================================================
static std::atomic<size_t> total_allocated_bytes{0};
static std::atomic<size_t> total_deallocated_bytes{0};

template <class T>
struct CustomAllocator {
    typedef T value_type;

    CustomAllocator() = default;
    
    template <class U> 
    constexpr CustomAllocator(const CustomAllocator<U>&) noexcept {}

    T* allocate(std::size_t n) {
        if (n > std::size_t(-1) / sizeof(T)) throw std::bad_alloc();
        if (auto p = static_cast<T*>(std::malloc(n * sizeof(T)))) {
            // 追蹤配置的位元組
            total_allocated_bytes += n * sizeof(T);
            return p;
        }
        throw std::bad_alloc();
    }

    void deallocate(T* p, std::size_t n) noexcept {
        // 追蹤釋放的位元組
        total_deallocated_bytes += n * sizeof(T);
        std::free(p);
    }
};

template <class T, class U>
bool operator==(const CustomAllocator<T>&, const CustomAllocator<U>&) { return true; }

template <class T, class U>
bool operator!=(const CustomAllocator<T>&, const CustomAllocator<U>&) { return false; }

// 提供給 Python 呼叫的 API
size_t get_bytes() { return total_allocated_bytes - total_deallocated_bytes; }
size_t get_allocated() { return total_allocated_bytes; }
size_t get_deallocated() { return total_deallocated_bytes; }

// ==============================================================================
// 2. Matrix 類別 (將 std::vector 套用 CustomAllocator)
// ==============================================================================
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
    // 使用自訂的記憶體分配器
    std::vector<double, CustomAllocator<double>> m_buffer;
};

// ==============================================================================
// 3. 矩陣運算實作
// ==============================================================================
Matrix multiply_naive(const Matrix& mat1, const Matrix& mat2) {
    if (mat1.ncol() != mat2.nrow()) throw std::invalid_argument("Dimension mismatch");
    Matrix result(mat1.nrow(), mat2.ncol());
    
    for (size_t i = 0; i < mat1.nrow(); ++i) {
        for (size_t j = 0; j < mat2.ncol(); ++j) {
            double sum = 0.0;
            for (size_t k = 0; k < mat1.ncol(); ++k) {
                sum += mat1(i, k) * mat2(k, j);
            }
            result(i, j) = sum;
        }
    }
    return result;
}

Matrix multiply_tile(const Matrix& mat1, const Matrix& mat2, size_t tsize) {
    if (mat1.ncol() != mat2.nrow()) throw std::invalid_argument("Dimension mismatch");
    Matrix result(mat1.nrow(), mat2.ncol());
    if (tsize == 0) return multiply_naive(mat1, mat2);

    for (size_t i0 = 0; i0 < mat1.nrow(); i0 += tsize) {
        size_t imax = std::min(i0 + tsize, mat1.nrow());
        
        for (size_t j0 = 0; j0 < mat2.ncol(); j0 += tsize) {
            size_t jmax = std::min(j0 + tsize, mat2.ncol());
            
            for (size_t k0 = 0; k0 < mat1.ncol(); k0 += tsize) {
                size_t kmax = std::min(k0 + tsize, mat1.ncol());

                for (size_t i = i0; i < imax; ++i) {
                    for (size_t k = k0; k < kmax; ++k) {
                        double v1 = mat1(i, k);
                        for (size_t j = j0; j < jmax; ++j) {
                            result(i, j) += v1 * mat2(k, j);
                        }
                    }
                }
            }
        }
    }
    return result;
}

Matrix multiply_mkl(const Matrix& mat1, const Matrix& mat2) {
    if (mat1.ncol() != mat2.nrow()) throw std::invalid_argument("Dimension mismatch");
    Matrix result(mat1.nrow(), mat2.ncol());

    cblas_dgemm(
        CblasRowMajor, CblasNoTrans, CblasNoTrans,
        mat1.nrow(), mat2.ncol(), mat1.ncol(),
        1.0, mat1.data(), mat1.ncol(),
        mat2.data(), mat2.ncol(),
        0.0, result.data(), result.ncol()
    );
    return result;
}

// ==============================================================================
// 4. Pybind11 綁定模組
// ==============================================================================
PYBIND11_MODULE(_matrix, m) {
    m.doc() = "Matrix multiplication module with memory tracking";

    // 綁定三個記憶體追蹤函式
    m.def("bytes", &get_bytes, "Get currently used memory in bytes");
    m.def("allocated", &get_allocated, "Get total allocated memory in bytes");
    m.def("deallocated", &get_deallocated, "Get total deallocated memory in bytes");

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