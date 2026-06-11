#include "matrix.hpp"
#include <stdexcept>
#include <algorithm>

#ifdef __APPLE__
#include <Accelerate/Accelerate.h>
#else
#include <mkl.h>
#endif

Matrix multiply_naive(Matrix const &mat1, Matrix const &mat2){
    if(mat1.ncol() != mat2.nrow()){
        throw std::runtime_error("Incompatible matrix dimensions");
    }
    Matrix C(mat1.nrow(), mat2.ncol());
    for(size_t i=0; i<mat1.nrow(); i++){
        for(size_t j=0; j<mat2.ncol(); j++){
            double sum = 0.0;
            for(size_t k=0; k<mat1.ncol(); k++){
                sum += mat1(i,k) * mat2(k,j);
            }
            C(i,j) = sum;
        }
    }
    return C;
}

Matrix multiply_mkl(Matrix const &mat1, Matrix const &mat2){
    if(mat1.ncol() != mat2.nrow()){
        throw std::runtime_error("Incompatible matrix dimensions");
    }
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

Matrix multiply_tile(Matrix const &mat1, Matrix const &mat2, size_t tsize){
    if(mat1.ncol() != mat2.nrow()){
        throw std::runtime_error("Incompatible matrix dimensions");
    }
    Matrix ret(mat1.nrow(), mat2.ncol());
    for(size_t i_0=0; i_0<mat1.nrow(); i_0 += tsize){
        for(size_t j_0=0; j_0<mat2.ncol(); j_0 += tsize){
            for(size_t k_0=0; k_0<mat1.ncol(); k_0 += tsize){
                size_t i_end = std::min(i_0 + tsize, mat1.nrow());
                size_t j_end = std::min(j_0 + tsize, mat2.ncol());
                size_t k_end = std::min(k_0 + tsize, mat1.ncol());
                for(size_t i=i_0; i<i_end; i++){
                    for(size_t k=k_0; k<k_end; k++){
                        double r = mat1(i, k);
                        for(size_t j=j_0; j<j_end; j++){
                            ret(i, j) += r * mat2(k, j);
                        }
                    }
                }
            }
        }
    }
    return ret;
}
