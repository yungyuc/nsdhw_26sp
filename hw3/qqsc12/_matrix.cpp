#include <iostream>

#include <vector>

#include <stdexcept>

#include <algorithm>

#include <mkl.h>

#include <pybind11/pybind11.h>

#include <pybind11/operators.h>

#include <pybind11/stl.h> // 確保支援 std::pair



namespace py = pybind11;



class Matrix {

public:

    Matrix(size_t nrow, size_t ncol) : m_nrow(nrow), m_ncol(ncol), m_data(nrow * ncol, 0) {}



    double  operator()(size_t row, size_t col) const { return m_data[row * m_ncol + col]; }

    double& operator()(size_t row, size_t col)       { return m_data[row * m_ncol + col]; }



    bool operator==(const Matrix& other) const {

        return m_nrow == other.m_nrow && m_ncol == other.m_ncol && m_data == other.m_data;

    }



    size_t nrow() const { return m_nrow; }

    size_t ncol() const { return m_ncol; }

   

    // 同時提供 const 與非 const 的數據指標，防止 MKL 調用時發生錯誤

    double* data() { return m_data.data(); }

    const double* data() const { return m_data.data(); }



private:

    size_t m_nrow, m_ncol;

    std::vector<double> m_data;

};


// 故意讓 Naive 變慢的順序 (i-j-k)
Matrix multiply_naive(const Matrix& m1, const Matrix& m2) {
    if (m1.ncol() != m2.nrow()) throw std::invalid_argument("Dimension mismatch");
    Matrix res(m1.nrow(), m2.ncol());
    for (size_t i = 0; i < m1.nrow(); ++i) {
        for (size_t j = 0; j < m2.ncol(); ++j) {
            double sum = 0;
            for (size_t k = 0; k < m1.ncol(); ++k) {
                sum += m1(i, k) * m2(k, j); // 這裡對 m2 是跳躍存取，會很慢
            }
            res(i, j) = sum;
        }
    }
    return res;
}


// ... 前面 Matrix 類別保持不變 ...

Matrix multiply_tile(const Matrix& m1, const Matrix& m2, size_t tsize) {
    if (m1.ncol() != m2.nrow()) throw std::invalid_argument("Dimension mismatch");

    const size_t nrow1 = m1.nrow();
    const size_t ncol1 = m1.ncol();
    const size_t ncol2 = m2.ncol();
    Matrix res(nrow1, ncol2);
    if (tsize == 0) return multiply_naive(m1, m2);

    const double* m1_ptr = m1.data();
    const double* m2_ptr = m2.data();
    double* res_ptr = res.data();

    // 調整區塊順序：i0 -> j0 -> k0
    for (size_t i0 = 0; i0 < nrow1; i0 += tsize) {
        size_t i_end = std::min(i0 + tsize, nrow1);
        for (size_t j0 = 0; j0 < ncol2; j0 += tsize) {
            size_t j_end = std::min(j0 + tsize, ncol2);
            for (size_t k0 = 0; k0 < ncol1; k0 += tsize) {
                size_t k_end = std::min(k0 + tsize, ncol1);

                // 核心運算
                for (size_t i = i0; i < i_end; ++i) {
                    size_t i_off = i * ncol1;
                    size_t res_off = i * ncol2;
                    for (size_t k = k0; k < k_end; ++k) {
                        // 提早取出 m1 的值
                        double r = m1_ptr[i_off + k];
                        size_t k_off = k * ncol2;
                        
                        // 最內層：讓編譯器看到這是連續的向量運算
                        for (size_t j = j0; j < j_end; ++j) {
                            res_ptr[res_off + j] += r * m2_ptr[k_off + j];
                        }
                    }
                }
            }
        }
    }
    return res;
}

Matrix multiply_mkl(const Matrix& m1, const Matrix& m2) {

    if (m1.ncol() != m2.nrow()) throw std::invalid_argument("Dimension mismatch");

    Matrix res(m1.nrow(), m2.ncol());

   

    // 直接使用 m1.data() 與 m2.data()，不用 const_cast 以確保安全

    cblas_dgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans,

                m1.nrow(), m2.ncol(), m1.ncol(),

                1.0, m1.data(), m1.ncol(),

                m2.data(), m2.ncol(),

                0.0, res.data(), res.ncol());

    return res;

}


#include <dlfcn.h> // 加入這行

PYBIND11_MODULE(_matrix, m) {
    // 暴力破解：在模組載入時，直接用 dlopen 把 MKL 核心庫載入全域符號表
    // 這樣不論是哪個進程呼叫，都能找到符號
    dlopen("libmkl_rt.so", RTLD_GLOBAL | RTLD_LAZY);

    py::class_<Matrix>(m, "Matrix")
        .def(py::init<size_t, size_t>())
        .def_property_readonly("nrow", &Matrix::nrow)
        .def_property_readonly("ncol", &Matrix::ncol)
        .def(py::self == py::self)
        .def("__getitem__", [](const Matrix &mat, std::pair<size_t, size_t> idx) {
            if (idx.first >= mat.nrow() || idx.second >= mat.ncol())
                throw py::index_error("Index out of range");
            return mat(idx.first, idx.second);
        })
        .def("__setitem__", [](Matrix &mat, std::pair<size_t, size_t> idx, double val) {
            if (idx.first >= mat.nrow() || idx.second >= mat.ncol())
                throw py::index_error("Index out of range");
            mat(idx.first, idx.second) = val;
        });

    m.def("multiply_naive", &multiply_naive);
    m.def("multiply_tile", &multiply_tile);
    m.def("multiply_mkl", &multiply_mkl);
}