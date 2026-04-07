import _matrix
import pytest
import math

def test_basic():
    mat = _matrix.Matrix(3, 5)
    assert mat.nrow == 3
    assert mat.ncol == 5

def test_set_get():
    mat = _matrix.Matrix(3, 5)
    mat[0, 0] = 1
    mat[1, 2] = 5
    mat[2, 4] = 10
    assert mat[0, 0] == 1
    assert mat[1, 2] == 5
    assert mat[2, 4] == 10

def test_multiplication():
    mat1 = _matrix.Matrix(10, 15)
    mat2 = _matrix.Matrix(15, 20)
    for i in range(10):
        for j in range(15):
            mat1[i, j] = i + j + 1
    for i in range(15):
        for j in range(20):
            mat2[i, j] = i * j + 1
    result_naive = _matrix.multiply_naive(mat1, mat2)
    result_tile = _matrix.multiply_tile(mat1, mat2, 4)
    result_mkl = _matrix.multiply_mkl(mat1, mat2)
    assert result_naive.nrow == 10
    assert result_naive.ncol == 20
    assert result_tile.nrow == 10
    assert result_tile.ncol == 20
    assert result_mkl.nrow == 10
    assert result_mkl.ncol == 20
    assert result_naive == result_tile
    assert result_naive == result_mkl

def test_large_matrix_tile():
    size = 500
    mat1 = _matrix.Matrix(size, size)
    mat2 = _matrix.Matrix(size, size)
    for i in range(size):
        for j in range(size):
            mat1[i, j] = i + j + 1
            mat2[i, j] = i * j + 1
    result = _matrix.multiply_tile(mat1, mat2, 16)
    assert result.nrow == size
    assert result.ncol == size