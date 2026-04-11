#include <vector>
#include <cstddef>
#include <stdexcept>
#include <mkl.h>

class Matrix
{
private:
    size_t m_rows;
    size_t m_cols;
    std::vector<double> m_data;    
public:
    Matrix(size_t rows, size_t cols) : m_rows(rows), m_cols(cols), m_data(rows * cols, 0.0) {};
    ~Matrix() = default;
    size_t rows() const { return m_rows; }
    size_t cols() const { return m_cols; }
    double operator()(size_t row, size_t col) const { return m_data[row * m_cols + col]; }
    double & operator()(size_t row, size_t col) { return m_data[row * m_cols + col]; }
    bool operator==(const Matrix & other) const {
        return m_rows == other.m_rows && m_cols == other.m_cols && m_data == other.m_data;
    }
    double const * data() const { return m_data.data(); }
    double * data() { return m_data.data(); }
};

Matrix multiply_naive(const Matrix & A, const Matrix & B);
Matrix multiply_tile(const Matrix & A, const Matrix & B, size_t tile_size);
Matrix multiply_mkl(const Matrix & A, const Matrix & B);
