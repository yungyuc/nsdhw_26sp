#ifndef MATRIX_HPP
#define MATRIX_HPP

#include <vector>
#include <cstddef>
#include <stdexcept>
#include "alloc.hpp"

class Matrix {

public:

    Matrix(size_t nrow, size_t ncol)
      : m_nrow(nrow), m_ncol(ncol), m_buffer(nrow * ncol, 0.0){}

    Matrix(size_t nrow, size_t ncol, std::vector<double, CustomAllocator<double>> const & vec)
      : m_nrow(nrow), m_ncol(ncol), m_buffer(vec){}

    Matrix & operator=(std::vector<double, CustomAllocator<double>> const & vec)
    {
        if(m_nrow && m_ncol){
            if (size() != vec.size())
            {
                throw std::out_of_range("number of elements mismatch");
        
            }
        }

        m_buffer = vec;

        return *this;
    }

    Matrix(Matrix const & other)
      : m_nrow(other.m_nrow), m_ncol(other.m_ncol), m_buffer(other.m_buffer){}

    Matrix & operator=(Matrix const & other)
    {
        if (this == &other) { return *this;}
        m_buffer = other.m_buffer;

        return *this;
    }

    Matrix(Matrix && other)
      : m_nrow(other.m_nrow), m_ncol(other.m_ncol)
    {
        m_buffer = std::move(other.m_buffer);
    }

    Matrix & operator=(Matrix && other)
    {
        if (this == &other) { return *this; }
        m_buffer = std::move(other.m_buffer);
        m_nrow = other.m_nrow;
        m_ncol = other.m_ncol;

        return *this;
    }

    ~Matrix() = default;

    double   operator() (size_t row, size_t col) const
    {
        return m_buffer[index(row, col)];
    }
    double & operator() (size_t row, size_t col)
    {
        return m_buffer[index(row, col)];
    }

    size_t nrow() const { return m_nrow; }
    size_t ncol() const { return m_ncol; }
    size_t size() const { return m_nrow * m_ncol; } 
    double* data() {return m_buffer.data(); }   
    const double* data() const { return m_buffer.data(); }

private:

    size_t index(size_t row, size_t col) const
    {
        return row + col * m_nrow; //colunm-major
    }

    size_t m_nrow = 0;
    size_t m_ncol = 0;
    std::vector<double, CustomAllocator<double>> m_buffer;
};

#endif