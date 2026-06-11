#ifndef MATRIX_HPP
#define MATRIX_HPP

#include <cstddef>
#include <vector>
#include <stdexcept>

class Matrix {
private:
    size_t m_nrow;
    size_t m_ncol;
    std::vector<double> m_buffer;

public:
    Matrix(size_t nrow, size_t ncol) : m_nrow(nrow), m_ncol(ncol), m_buffer(nrow * ncol, 0.0) {}

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

    double* get_buffer() {
        return m_buffer.data();
    }

    bool operator==(const Matrix& other) const {
        if (m_nrow != other.m_nrow || m_ncol != other.m_ncol) return false;
        return m_buffer == other.m_buffer;
    }
};

Matrix multiply_naive(Matrix const & mat1, Matrix const & mat2);
Matrix multiply_tile(Matrix const & mat1, Matrix const & mat2, size_t tsize);
Matrix multiply_mkl(Matrix const & mat1, Matrix const & mat2);

#endif // MATRIX_HPP
