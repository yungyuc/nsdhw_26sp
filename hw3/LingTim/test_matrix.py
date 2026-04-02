import _matrix
import pytest

def test_basic_creation():
    mat = _matrix.Matrix(10, 10)
    assert mat.nrow == 10
    assert mat.ncol == 10
    assert mat[0, 0] == 0.0 

def test_multiplication():
    size = 100
    mat1 = _matrix.Matrix(size, size)
    mat2 = _matrix.Matrix(size, size)

    for i in range(size):
        for j in range(size):
            mat1[i, j] = i * 2 + j
            mat2[i, j] = i + j * 2

    res_naive = _matrix.multiply_naive(mat1, mat2)
    res_tile = _matrix.multiply_tile(mat1, mat2, 16)
    res_mkl = _matrix.multiply_mkl(mat1, mat2)

    assert res_naive == res_tile
    assert res_naive == res_mkl
