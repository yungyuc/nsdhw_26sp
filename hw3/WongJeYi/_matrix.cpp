#include "mkl.h"
#include <pybind11/pybind11.h>
#include <algorithm>

namespace py = pybind11;
class Matrix{       
        
    public:
        int m_nrow;
        int m_ncol;
        double* mat;
        Matrix()=default;
        ~Matrix(){
            if(mat){
                std::free(mat);
            }
        };
        // Constructor
        Matrix(int nrow, int ncol) : m_nrow(nrow), m_ncol(ncol) {
            mat = (double*)std::calloc(nrow * ncol, sizeof(double));
            if (!mat) throw std::runtime_error("Memory allocation failed");
        }

        // Copy Constructor
        Matrix(const Matrix& other) : m_nrow(other.m_nrow), m_ncol(other.m_ncol) {
            if (other.mat) {
                mat = (double*)std::malloc(m_nrow * m_ncol * sizeof(double));
                std::copy_n(other.mat, m_nrow * m_ncol, mat);
            }
        }
        bool operator==(const Matrix& other) const {
            if (m_nrow != other.m_nrow || m_ncol != other.m_ncol) return false;
            for (int i = 0; i < m_nrow * m_ncol; ++i) {
                if (mat[i] != other.mat[i]) return false;
            }
            return true;
        }
        static int findLCF(int s){
            if (s <= 1) return s;
            for(int i=2;i<s;i++){
                if(s%i==0){
                    return i;
                    break;
                }
            }
            return s;
        }

        static Matrix multiply_naive(Matrix mat1, Matrix mat2){
            if(mat1.m_ncol!=mat2.m_nrow){
                throw std::runtime_error("Row and column mismatch");
            }
            if(mat1.m_nrow==0){
                throw std::runtime_error("empty mat1");
            }
            
            double* m1=mat1.mat;
            double* m2=mat2.mat;
            Matrix result=Matrix(mat1.m_nrow,mat2.m_ncol);
            //i =row, j=col
            //mat[i][j]
            int M=mat1.m_nrow;
            int N=mat2.m_ncol;
            int K=mat1.m_ncol;

            for(int i=0;i<M;i++){
                for(int j=0;j<N;j++){
                    for(int k=0;k<K;k++){
                        result.mat[i*N+j] += m1[i*K+k]*m2[k*N+j];
                    }
                }
            }
            return result;
        }

        static Matrix multiply_tile(Matrix mat1, Matrix mat2, int tsize){
            if(mat1.m_ncol!=mat2.m_nrow){
                throw std::runtime_error("Row and column mismatch");
            }
            if(mat1.m_nrow==0){
                throw std::runtime_error("empty mat1");
            }
            double* m1=mat1.mat;
            double* m2=mat2.mat;
            Matrix result=Matrix(mat1.m_nrow,mat2.m_ncol);
            //i =row, j=col
            //mat[i][j]
            //Stride = LCF
            if(tsize==0){
                tsize=1;
            }
            
            int M=mat1.m_nrow;
            int N=mat2.m_ncol;
            int K=mat1.m_ncol;
            for(int i=0; i<M;i+=tsize){
                for(int k=0;k<K;k+=tsize){
                    for(int j=0;j<N;j+=tsize){
                        for(int ii=i;ii<std::min(i+tsize,M);ii++){
                            for(int kk=k;kk<std::min(k+tsize,K);kk++){
                                for(int jj=j;jj<std::min(j+tsize,N);jj++){
                                    result.mat[ii*N+jj]+=m1[(kk+K*ii)]*m2[kk*N+jj];
                                }
                            }
                        }

                    }
                }
            }
            return result;

        }

        static Matrix multiply_mkl(Matrix mat1, Matrix mat2){
            if (mat1.m_ncol != mat2.m_nrow) throw std::runtime_error("Dimension mismatch");

            double* A=mat1.mat;
            double* B=mat2.mat;
            Matrix result=Matrix(mat1.m_nrow,mat2.m_ncol);
            int m=mat1.m_nrow;
            int n=mat2.m_ncol;
            int k=mat1.m_ncol;
            double* C=result.mat;
            
            double alpha = 1.0; 
            double beta = 0.0;

            cblas_dgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans, 
                        m, n, k, alpha, A, k, B, n, beta, C, n);

            return result;
        }
};
PYBIND11_MODULE(_matrix, m) {
    py::class_<Matrix>(m, "Matrix")
    .def(py::init<int, int>())
    .def_property_readonly("nrow",[](const Matrix &m) { return m.m_nrow; })
    .def_property_readonly("ncol",[](const Matrix &m) { return m.m_ncol; })
    .def("__getitem__", [](const Matrix &m, std::pair<int, int> idx) {
        if (idx.first >= m.m_nrow || idx.second >= m.m_ncol) throw py::index_error();
        return m.mat[idx.first * m.m_ncol + idx.second];
    })
    .def("__setitem__", [](Matrix &m, std::pair<int, int> idx, double val) {
        if (idx.first >= m.m_nrow || idx.second >= m.m_ncol) throw py::index_error();
        m.mat[idx.first * m.m_ncol + idx.second] = val;
    })
   .def("__eq__", &Matrix::operator==);
    m.def("multiply_naive", &Matrix::multiply_naive, "Multiply Naive", py::arg("mat1"),py::arg("mat2"));
    
    m.def("multiply_tile", &Matrix::multiply_tile, "Multiply tile", py::arg("mat1"),py::arg("mat2"),py::arg("tsize"));
    
    m.def("multiply_mkl", &Matrix::multiply_mkl, "Multiply MKL", py::arg("mat1"),py::arg("mat2"));
}