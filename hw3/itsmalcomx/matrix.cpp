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


static constexpr size_t KC = 128;
static constexpr size_t NC = 500;  

Matrix multiply_tile(const Matrix &A, const Matrix &B, size_t tsize) {
    if (A.ncol() != B.nrow())
        throw std::invalid_argument("Matrix dimensions do not match");
    if (tsize == 0)
        throw std::invalid_argument("Tile size must be > 0");

    const size_t M = A.nrow(), K = A.ncol(), N = B.ncol();
    Matrix C(M, N);
    const double *a = A.data(), *b = B.data();
    double       *c = C.data();

  
    const size_t MC = std::max(tsize, size_t(64));

  
    std::vector<double> bpack(KC * NC);
    
    std::vector<double> apack(MC * KC);

    for (size_t k0 = 0; k0 < K; k0 += KC) {
        const size_t kEnd = std::min(k0 + KC, K);
        const size_t kLen = kEnd - k0;

        for (size_t j0 = 0; j0 < N; j0 += NC) {
            const size_t jEnd = std::min(j0 + NC, N);
            const size_t jLen = jEnd - j0;

            
            for (size_t ki = 0; ki < kLen; ++ki)
                std::memcpy(bpack.data() + ki * jLen,
                            b + (k0 + ki) * N + j0,
                            jLen * sizeof(double));

            for (size_t i0 = 0; i0 < M; i0 += MC) {
                const size_t iEnd = std::min(i0 + MC, M);
                const size_t iLen = iEnd - i0;

             
                for (size_t ii = 0; ii < iLen; ++ii)
                    std::memcpy(apack.data() + ii * kLen,
                                a + (i0 + ii) * K + k0,
                                kLen * sizeof(double));

               
                for (size_t ii = 0; ii < iLen; ++ii) {
                    const double *ai = apack.data() + ii * kLen;
                    double       *ci = c + (i0 + ii) * N + j0;
                    const double *bp = bpack.data();

                    size_t ki = 0;
                    
                    for (; ki + 4 <= kLen; ki += 4) {
                        const __m256d va0 = _mm256_set1_pd(ai[ki+0]);
                        const __m256d va1 = _mm256_set1_pd(ai[ki+1]);
                        const __m256d va2 = _mm256_set1_pd(ai[ki+2]);
                        const __m256d va3 = _mm256_set1_pd(ai[ki+3]);
                        const double *b0 = bp + (ki+0)*jLen;
                        const double *b1 = bp + (ki+1)*jLen;
                        const double *b2 = bp + (ki+2)*jLen;
                        const double *b3 = bp + (ki+3)*jLen;
                        size_t j = 0;
                        for (; j + 4 <= jLen; j += 4) {
                            __m256d vc = _mm256_loadu_pd(ci+j);
                            vc = _mm256_fmadd_pd(va0, _mm256_loadu_pd(b0+j), vc);
                            vc = _mm256_fmadd_pd(va1, _mm256_loadu_pd(b1+j), vc);
                            vc = _mm256_fmadd_pd(va2, _mm256_loadu_pd(b2+j), vc);
                            vc = _mm256_fmadd_pd(va3, _mm256_loadu_pd(b3+j), vc);
                            _mm256_storeu_pd(ci+j, vc);
                        }
                        for (; j < jLen; ++j)
                            ci[j] += ai[ki+0]*b0[j] + ai[ki+1]*b1[j]
                                   + ai[ki+2]*b2[j] + ai[ki+3]*b3[j];
                    }
                    for (; ki < kLen; ++ki) {
                        const double aik = ai[ki];
                        const double *bk = bp + ki*jLen;
                        for (size_t j = 0; j < jLen; ++j)
                            ci[j] += aik * bk[j];
                    }
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
