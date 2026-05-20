#include <cstddef>
#include <algorithm>
#include <pybind11/pybind11.h>
#include <pybind11/operators.h>
#include <mkl_cblas.h>
#include <atomic>
#include <vector>
namespace py = pybind11;


std::atomic<size_t> total_allocated(0);
std::atomic<size_t> total_deallocated(0);

size_t bytes(){
    return total_allocated - total_deallocated;
}
size_t allocated(){
    return total_allocated;
}
size_t deallocated(){
    return total_deallocated;
}


template <class T>
struct CustomAllocator{
    using value_type = T;
    CustomAllocator() = default;

    template <class U>
    constexpr CustomAllocator(const CustomAllocator<U>&) noexcept {}

    T* allocate(std::size_t n){
        if(n > std::numeric_limits<std::size_t>::max() / sizeof(T)){
            throw std::bad_alloc();
        }

        std::size_t bytes = n * sizeof(T);
        T* p = static_cast<T*>(::operator new(bytes));
        total_allocated += bytes;
        return p;
    }

    void deallocate(T* p, std::size_t n) noexcept {
        total_deallocated += n * sizeof(T);
        ::operator delete(p);
    }

    template <class U>
    bool operator==(const CustomAllocator<U>&) const noexcept {
        return true;
    }

};


class Matrix {
    private:
        size_t m_nrow;
        size_t m_ncol;
        std::vector<double, CustomAllocator<double>> m_buffer; // 1D array, len =nrow * ncol

    public:
        // Constructor
        Matrix(size_t nrow, size_t ncol) : m_nrow(nrow), m_ncol(ncol), m_buffer(nrow * ncol, 0.0) {}
        
        // Destructor


        // 存取
        size_t nrow() const { return m_nrow; }
        size_t ncol() const { return m_ncol; }
        double& operator()(size_t i, size_t j) { 
            return m_buffer[i * m_ncol + j]; 
        }
        double operator()(size_t i, size_t j) const { 
            return m_buffer[i * m_ncol + j];
        }

        const double* get_buffer() const { 
            return m_buffer.data(); 
        }

        double* get_buffer(){
            return m_buffer.data();
        }

        // compare
        /*        // === 比較 ===
        operator==(other) -> bool:
            如果大小不同 回傳 false
            逐一比較每個元素
            全部相同 回傳 true*/
        bool operator==(const Matrix& other) const {
            if(m_nrow != other.m_nrow || m_ncol != other.m_ncol) return false;
            return m_buffer == other.m_buffer;
        }

        // copy constructor

        // copy assignment

        // move constructor

        // move assignment

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


// Tiled matrix multiplication
/*
加 size_t tsize 參數
6 層迴圈（外 3 層切塊 + 內 3 層算小塊）
min() 處理邊界
C[i][k] 要累加（因為多個 j-block 會貢獻同一個位置
*/
Matrix multiply_tile(Matrix const &mat1, Matrix const &mat2, size_t tsize){
    if(mat1.ncol() != mat2.nrow()){
        throw std::runtime_error("Incompatible matrix dimensions");
    }
    Matrix ret(mat1.nrow(),mat2.ncol());
    for(size_t i_0=0; i_0<mat1.nrow(); i_0 += tsize){
        for(size_t k_0=0; k_0<mat2.ncol(); k_0 += tsize){
            for(size_t j_0=0; j_0<mat1.ncol(); j_0 += tsize){
                size_t i_end = std::min(i_0 + tsize, mat1.nrow());
                size_t k_end = std::min(k_0 + tsize, mat2.ncol());
                size_t j_end = std::min(j_0 + tsize, mat1.ncol());
                for(size_t i=i_0; i<i_end; i++){
                    for(size_t k=k_0; k<k_end; k++){
                        double sum = ret(i,k);
                        for(size_t j=j_0; j<j_end; j++){
                            sum += mat1(i,j) * mat2(j,k);
                        }
                        ret(i,k) = sum;
                    }
                }
            }
        }
    }
    return ret;
}

PYBIND11_MODULE(_matrix, m){
    m.doc() = "Matrix class for homework 4";

    m.def("bytes", &bytes, "Returns the current number of bytes in use");
    m.def("allocated", &allocated, "Returns the total number of bytes allocated");
    m.def("deallocated", &deallocated, "Returns the total number of bytes deallocated");
    
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
    m.def("multiply_tile", &multiply_tile);
}