#include "matrix.hpp"

Matrix multiply_naive(const Matrix & A, const Matrix & B)
{
    if (A.cols() != B.rows()) throw std::invalid_argument("Incompatible matrix dimensions.");
    Matrix C(A.rows(), B.cols());
    for (size_t i = 0; i < A.rows(); ++i) {
        for (size_t j = 0; j < B.cols(); ++j) {
            double sum = 0.0;
            for (size_t k = 0; k < A.cols(); ++k) {
                sum += A(i, k) * B(k, j);
            }
            C(i, j) = sum;
        }
    }
    return C;
}

Matrix multiply_tile(const Matrix & A, const Matrix & B, size_t tile_size)
{
    if (A.cols() != B.rows()) throw std::invalid_argument("Incompatible matrix dimensions.");
    Matrix C(A.rows(), B.cols());
    for (size_t ii = 0; ii < A.rows(); ii += tile_size) {
        for (size_t jj = 0; jj < B.cols(); jj += tile_size) {
            for (size_t kk = 0; kk < A.cols(); kk += tile_size) {
                size_t i_max = std::min(ii + tile_size, A.rows());
                size_t j_max = std::min(jj + tile_size, B.cols());
                size_t k_max = std::min(kk + tile_size, A.cols());
                for (size_t i = ii; i < i_max; ++i) {
                    for (size_t j = jj; j < j_max; ++j) {
                        double sum = 0.0;
                        for (size_t k = kk; k < k_max; ++k) {
                            sum += A(i, k) * B(k, j);
                        }
                        C(i, j) += sum;
                    }
                }
            }
        }
    }
    return C;
}

Matrix multiply_mkl(const Matrix & A, const Matrix & B)
{
    if (A.cols() != B.rows()) throw std::invalid_argument("Incompatible matrix dimensions.");
    Matrix C(A.rows(), B.cols());
    cblas_dgemm(
        CblasRowMajor, CblasNoTrans, CblasNoTrans,
        A.rows(), B.cols(), A.cols(),
        1.0,
        A.data(), A.cols(),
        B.data(), B.cols(),
        0.0,
        C.data(), C.cols()
    );
    return C;
}