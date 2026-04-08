#include <pybind11/pybind11.h>
#include <cblas.h>
#include <vector>
#include <stdexcept>
#include <cmath>
#include <cstring>
#include <immintrin.h>

namespace py = pybind11;

class Matrix {
public:
    Matrix(size_t nrow, size_t ncol)
        : m_nrow(nrow), m_ncol(ncol), m_data(nrow * ncol, 0.0) {}

    Matrix(const Matrix &) = default;
    Matrix &operator=(const Matrix &) = default;

    size_t nrow() const { return m_nrow; }
    size_t ncol() const { return m_ncol; }

    double  operator()(size_t i, size_t j) const { return m_data[i * m_ncol + j]; }
    double &operator()(size_t i, size_t j)       { return m_data[i * m_ncol + j]; }

    double       *data()       { return m_data.data(); }
    const double *data() const { return m_data.data(); }

    bool operator==(const Matrix &o) const {
        if (m_nrow != o.m_nrow || m_ncol != o.m_ncol) return false;
        for (size_t k = 0; k < m_nrow * m_ncol; ++k)
            if (std::fabs(m_data[k] - o.m_data[k]) > 1e-8) return false;
        return true;
    }

private:
    size_t m_nrow, m_ncol;
    std::vector<double> m_data;
};


Matrix multiply_naive(const Matrix &A, const Matrix &B) {
    if (A.ncol() != B.nrow())
        throw std::invalid_argument("Matrix dimensions do not match");

    const size_t M = A.nrow(), K = A.ncol(), N = B.ncol();
    Matrix C(M, N);
    const double *a = A.data(), *b = B.data();
    double       *c = C.data();

    for (size_t i = 0; i < M; ++i)
        for (size_t k = 0; k < K; ++k) {
            const double aik = a[i*K + k];
            const double *bk = b + k*N;
            double       *ci = c + i*N;
            for (size_t j = 0; j < N; ++j)
                ci[j] += aik * bk[j];
        }
    return C;
}


static void axpy_avx2(double * __restrict__ c,
                      const double * __restrict__ b,
                      double alpha, size_t n) {
    const __m256d va = _mm256_set1_pd(alpha);
    size_t j = 0;
    for (; j + 16 <= n; j += 16) {
        _mm256_storeu_pd(c+j,    _mm256_fmadd_pd(va, _mm256_loadu_pd(b+j),    _mm256_loadu_pd(c+j)));
        _mm256_storeu_pd(c+j+4,  _mm256_fmadd_pd(va, _mm256_loadu_pd(b+j+4),  _mm256_loadu_pd(c+j+4)));
        _mm256_storeu_pd(c+j+8,  _mm256_fmadd_pd(va, _mm256_loadu_pd(b+j+8),  _mm256_loadu_pd(c+j+8)));
        _mm256_storeu_pd(c+j+12, _mm256_fmadd_pd(va, _mm256_loadu_pd(b+j+12), _mm256_loadu_pd(c+j+12)));
    }
    for (; j + 4 <= n; j += 4)
        _mm256_storeu_pd(c+j, _mm256_fmadd_pd(va, _mm256_loadu_pd(b+j), _mm256_loadu_pd(c+j)));
    for (; j < n; ++j)
        c[j] += alpha * b[j];
}


static constexpr size_t JBLOCK = 256;

Matrix multiply_tile(const Matrix &A, const Matrix &B, size_t tsize) {
    if (A.ncol() != B.nrow())
        throw std::invalid_argument("Matrix dimensions do not match");
    if (tsize == 0)
        throw std::invalid_argument("Tile size must be > 0");

    const size_t M = A.nrow(), K = A.ncol(), N = B.ncol();
    Matrix C(M, N);
    const double *a = A.data(), *b = B.data();
    double       *c = C.data();

   
    std::vector<double> bpack(tsize * JBLOCK);

    for (size_t J = 0; J < N; J += JBLOCK) {
        const size_t JMax = std::min(J + JBLOCK, N);
        const size_t JBlk = JMax - J;

        for (size_t k0 = 0; k0 < K; k0 += tsize) {
            const size_t kMax = std::min(k0 + tsize, K);
            const size_t kBlk = kMax - k0;

          
            for (size_t ki = 0; ki < kBlk; ++ki)
                std::memcpy(bpack.data() + ki * JBlk,
                            b + (k0 + ki) * N + J,
                            JBlk * sizeof(double));

          
            for (size_t i0 = 0; i0 < M; i0 += tsize) {
                const size_t iMax = std::min(i0 + tsize, M);
                const double *bp  = bpack.data();

                for (size_t i = i0; i < iMax; ++i) {
                    const double *ai = a + i * K + k0;
                    double       *ci = c + i * N + J;
                    for (size_t ki = 0; ki < kBlk; ++ki)
                        axpy_avx2(ci, bp + ki * JBlk, ai[ki], JBlk);
                }
            }
        }
    }
    return C;
}


Matrix multiply_mkl(const Matrix &A, const Matrix &B) {
    if (A.ncol() != B.nrow())
        throw std::invalid_argument("Matrix dimensions do not match");

    const int M = (int)A.nrow(), K = (int)A.ncol(), N = (int)B.ncol();
    Matrix C(M, N);

    cblas_dgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans,
                M, N, K,
                1.0, A.data(), K,
                     B.data(), N,
                0.0, C.data(), N);
    return C;
}

// pybind11 here

PYBIND11_MODULE(_matrix, m) {
    m.doc() = "Matrix multiplication: naive, tiled, BLAS DGEMM";

    py::class_<Matrix>(m, "Matrix")
        .def(py::init<size_t, size_t>(), py::arg("nrow"), py::arg("ncol"))
        .def_property_readonly("nrow", &Matrix::nrow)
        .def_property_readonly("ncol", &Matrix::ncol)
        .def("__getitem__",
             [](const Matrix &M, std::pair<size_t,size_t> idx) {
                 return M(idx.first, idx.second);
             })
        .def("__setitem__",
             [](Matrix &M, std::pair<size_t,size_t> idx, double v) {
                 M(idx.first, idx.second) = v;
             })
        .def("__eq__", &Matrix::operator==)
        ;

    m.def("multiply_naive", &multiply_naive, py::arg("A"), py::arg("B"));
    m.def("multiply_tile",  &multiply_tile,  py::arg("A"), py::arg("B"), py::arg("tsize"));
    m.def("multiply_mkl",   &multiply_mkl,   py::arg("A"), py::arg("B"));
}