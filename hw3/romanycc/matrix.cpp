#include <iostream>
#include <pybind11/pybind11.h>
#include <vector>
#include <stdexcept>
#ifdef __APPLE__
    #include <Accelerate/Accelerate.h>
#else
    #include <mkl.h>
#endif
using namespace std;
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
    vector<double> data_;
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
}