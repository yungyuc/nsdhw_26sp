import _matrix
import pytest
import math

def test_basic():
    size = 100
    mat1 = _matrix.Matrix(size, size)
    mat2 = _matrix.Matrix(size, size)
    
    # 填充資料
    for i in range(size):
        for j in range(size):
            val = float(i * size + j + 1)
            mat1[i, j] = val
            mat2[i, j] = val
    
    assert mat1.nrow == size
    assert mat1.ncol == size
    
    # 測試 Naive 與 MKL 結果是否一致
    ret_naive = _matrix.multiply_naive(mat1, mat2)
    ret_mkl = _matrix.multiply_mkl(mat1, mat2)
    
    for i in range(size):
        for j in range(size):
            assert math.isclose(ret_naive[i, j], ret_mkl[i, j], rel_tol=1e-9)

def test_tile():
    size = 200
    mat1 = _matrix.Matrix(size, size)
    mat2 = _matrix.Matrix(size, size)
    
    # 填充資料 (確保不是全 0)
    for i in range(size):
        for j in range(size):
            mat1[i, j] = 1.0
            mat2[i, j] = 2.0
            
    ret_tile = _matrix.multiply_tile(mat1, mat2, 16)
    ret_mkl = _matrix.multiply_mkl(mat1, mat2)
    
    for i in range(size):
        for j in range(size):
            assert math.isclose(ret_tile[i, j], ret_mkl[i, j], rel_tol=1e-9)