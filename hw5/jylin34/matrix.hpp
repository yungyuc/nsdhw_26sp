#ifndef MATRIX_HPP
#define MATRIX_HPP

#include <cstddef>
#include <vector>
#include <stdexcept>

// Memory tracking accessors (defined in implementation file).
size_t get_bytes();
size_t get_allocated();
size_t get_deallocated();

// Minimal CustomAllocator declaration used by Matrix.
template <class T>
struct CustomAllocator {
    using value_type = T;
    CustomAllocator() = default;
    template <class U>
    constexpr CustomAllocator(const CustomAllocator<U>&) noexcept {}
    T* allocate(std::size_t n);
    void deallocate(T* p, std::size_t n) noexcept;
};

template <class T, class U>
bool operator==(const CustomAllocator<T>&, const CustomAllocator<U>&);
template <class T, class U>
bool operator!=(const CustomAllocator<T>&, const CustomAllocator<U>&);

class Matrix {
private:
    size_t m_nrow;
    size_t m_ncol;
    std::vector<double, CustomAllocator<double>> m_buffer;

public:
    Matrix(size_t nrow, size_t ncol);
    ~Matrix();
    Matrix(const Matrix& other);
    Matrix& operator=(const Matrix& other);
    Matrix(Matrix&& other) noexcept;
    Matrix& operator=(Matrix&& other) noexcept;

    size_t nrow() const;
    size_t ncol() const;

    double& operator()(size_t i, size_t j);
    double operator()(size_t i, size_t j) const;

    double* get_buffer();
    const double* get_buffer() const;

    bool operator==(const Matrix& other) const;
};

Matrix multiply_naive(Matrix const &mat1, Matrix const &mat2);
Matrix multiply_mkl(Matrix const &mat1, Matrix const &mat2);
Matrix multiply_tile(const Matrix& A, const Matrix& B, size_t tile_size);

#endif // MATRIX_HPP
