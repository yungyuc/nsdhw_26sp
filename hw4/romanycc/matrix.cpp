#include <iostream>
#include <pybind11/pybind11.h>
#include <vector>
#include <stdexcept>
#include <limits>
#ifdef __APPLE__
    #include <Accelerate/Accelerate.h>
#else
    #include <mkl.h>
#endif
using namespace std;

static size_t g_allocated = 0;
static size_t g_deallocated = 0;
template <typename T>
struct CustomAllocator{
    using value_type = T;
    CustomAllocator() = default;
    template <class U>
    CustomAllocator(const CustomAllocator<U>&) noexcept {}
    T* allocate(std::size_t n){
        if (n > std::numeric_limits<std::size_t>::max() / sizeof(T))
            throw std::bad_alloc();
        size_t bytes = n * sizeof(T);
        T* p = static_cast<T*>(std::malloc(bytes));
        if (!p) throw std::bad_alloc();

        g_allocated += bytes; 
        return p;
    }
    void deallocate(T* p, std::size_t n) noexcept {
        size_t bytes = n * sizeof(T);
        g_deallocated += bytes; // 增加累計釋放
        std::free(p);
    }

};

template <class T, class U>
bool operator==(const CustomAllocator<T>&, const CustomAllocator<U>&) { return true; }
template <class T, class U>
bool operator!=(const CustomAllocator<T>&, const CustomAllocator<U>&) { return false; }


class Matrix{
public:
    Matrix(size_t nrow, size_t n_col) : rows_(nrow), cols_(n_col), data_(nrow * n_col, 0.0) {}
    double get_item(size_t r, size_t c) const{
        return data_[r * cols_ + c];
    }
    void set_item(size_t r, size_t c, double val){
        data_[r * cols_ + c] = val;
    }
    inline size_t nrow() const {return rows_;}
    inline size_t ncol() const {return cols_;}
    bool operator==(const Matrix &other) const{
        if (rows_ != other.rows_ || cols_ != other.cols_){
            return false;
        }
        for (size_t i = 0; i < data_.size(); ++i){
            if (abs(data_[i] - other.data_[i]) > 1e-9){
                return false;
            }
        }
        return true;
    }
    double* data() { return data_.data(); }
    const double* data() const { return data_.data(); }
    private:
    size_t rows_;
    size_t cols_;
    vector<double, CustomAllocator<double>> data_;
};
Matrix multiply_naive(const Matrix &A, const Matrix &B){
    if (A.ncol() != B.nrow()){
        throw invalid_argument("Incompatible matrix dimensions for multiplication");
    }
    Matrix C(A.nrow(), B.ncol());
    for (size_t i=0; i<A.nrow();++i){
        for (size_t j=0;j<B.ncol();++j){
            double sum=0;
            for (size_t k=0; k<A.ncol();++k){
                sum += A.get_item(i, k) * B.get_item(k, j);
            }
            C.set_item(i, j, sum);
        }
    }
    return C;

}
Matrix multiply_tile(const Matrix &A, const Matrix &B, int T){
    if (A.ncol() != B.nrow()){
        throw invalid_argument("Incompatible matrix dimensions for multiplication");
    }
    Matrix C(A.nrow(), B.ncol());
    for (size_t i=0; i<A.nrow(); i+=T){
        for (size_t j=0; j<B.ncol(); j+=T){
            for (size_t k=0; k<A.ncol(); k+=T){
                // Compute C(i:i+T, j:j+T) += A(i:i+T, k:k+T) * B(k:k+T, j:j+T)
                for (size_t ii=i; ii<min(i+T, A.nrow()); ++ii){
                    for (size_t jj=j; jj<min(j+T, B.ncol()); ++jj){
                        double sum = C.get_item(ii, jj);
                        for (size_t kk=k; kk<min(k+T, A.ncol()); ++kk){
                            sum += A.get_item(ii, kk) * B.get_item(kk, jj);
                        }
                        C.set_item(ii, jj, sum);
                    }
                }
            }
        }
    }
    return C;
}
Matrix multiply_mkl(const Matrix &A, const Matrix &B) {
    if (A.ncol() != B.nrow()) {
        throw invalid_argument("Incompatible matrix dimensions for multiplication");
    }

    Matrix C(A.nrow(), B.ncol());   
    cblas_dgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans,
                A.nrow(), B.ncol(), A.ncol(),
                1.0, A.data(), A.ncol(), B.data(), B.ncol(),
                0.0, C.data(), C.ncol());

    return C;
}
size_t get_bytes() {
    return g_allocated - g_deallocated;
}

size_t get_allocated() {
    return g_allocated;
}

size_t get_deallocated() {
    return g_deallocated;
}
PYBIND11_MODULE(_matrix, m){
    pybind11::class_<Matrix>(m, "Matrix")
        .def(pybind11::init<size_t, size_t>())
        .def_property_readonly("nrow", &Matrix::nrow)
        .def_property_readonly("ncol", &Matrix::ncol)
        .def("__getitem__", [](const Matrix &m, std::pair<size_t, size_t> idx) { 
            return m.get_item(idx.first, idx.second); 
        })
        .def("__setitem__", [](Matrix &m, std::pair<size_t, size_t> idx, double v) { 
            m.set_item(idx.first, idx.second, v); 
        })
        .def("__eq__", &Matrix::operator==);
    m.def("multiply_naive", &multiply_naive, "A naive implementation of matrix multiplication");
    m.def("multiply_tile", &multiply_tile, "A tiled implementation of matrix multiplication");
    m.def("multiply_mkl", &multiply_mkl, "A MKL implementation of matrix multiplication");
    m.def("bytes", &get_bytes);
    m.def("allocated", &get_allocated);
    m.def("deallocated", &get_deallocated);
}