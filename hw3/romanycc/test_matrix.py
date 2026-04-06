import _matrix
import pytest
import time
import random

def test_basic_properties():
    """測試矩陣建立與基本屬性讀取"""
    nrow, ncol = 3, 5
    mat = _matrix.Matrix(nrow, ncol)
    assert mat.nrow == nrow
    assert mat.ncol == ncol
    
    # 檢查預設值是否為 0
    for i in range(nrow):
        for j in range(ncol):
            assert mat[i, j] == 0.0

def test_get_set_item():
    """測試索引存取與賦值 mat[i, j]"""
    mat = _matrix.Matrix(2, 2)
    mat[0, 0] = 1.1
    mat[0, 1] = 2.2
    mat[1, 0] = 3.3
    mat[1, 1] = 4.4
    
    assert mat[0, 0] == 1.1
    assert mat[1, 1] == 4.4

def test_multiplication_correctness():
    """使用小矩陣手動驗證正確性"""
    # A = [[1, 2], [3, 4]] (2x2)
    # B = [[5, 6], [7, 8]] (2x2)
    # C = [[19, 22], [43, 50]]
    A = _matrix.Matrix(2, 2)
    B = _matrix.Matrix(2, 2)
    
    A[0, 0], A[0, 1] = 1, 2
    A[1, 0], A[1, 1] = 3, 4
    
    B[0, 0], B[0, 1] = 5, 6
    B[1, 0], B[1, 1] = 7, 8
    
    target = [[19.0, 22.0], [43.0, 50.0]]
    
    for func in [_matrix.multiply_naive, _matrix.multiply_mkl]:
        res = func(A, B)
        assert res.nrow == 2
        assert res.ncol == 2
        for i in range(2):
            for j in range(2):
                assert abs(res[i, j] - target[i][j]) < 1e-9

    # 測試 Tiling 版 (tsize=1)
    res_tile = _matrix.multiply_tile(A, B, 1)
    for i in range(2):
        for j in range(2):
            assert abs(res_tile[i, j] - target[i][j]) < 1e-9

def test_consistency_large():
    """大矩陣隨機測試，確保 Naive, Tile, MKL 三者結果完全一致"""
    size = 200
    tsize = 16
    A = _matrix.Matrix(size, size)
    B = _matrix.Matrix(size, size)
    
    for i in range(size):
        for j in range(size):
            A[i, j] = random.random()
            B[i, j] = random.random()
            
    res_naive = _matrix.multiply_naive(A, B)
    res_tile = _matrix.multiply_tile(A, B, tsize)
    res_mkl = _matrix.multiply_mkl(A, B)
    
    # 測試 __eq__ 是否運作正常
    assert res_naive == res_mkl
    assert res_tile == res_mkl
    assert res_naive == res_tile

def test_tiling_performance():
    """效能測試：確保 Tiling 版本確實比 Naive 快 (作業規定至少快 20%)"""
    size = 500
    tsize = 32
    A = _matrix.Matrix(size, size)
    B = _matrix.Matrix(size, size)
    
    # 初始化
    for i in range(size):
        for j in range(size):
            A[i, j] = i + j
            B[i, j] = i - j
            
    # 測量 Naive
    start = time.time()
    _matrix.multiply_naive(A, B)
    time_naive = time.time() - start
    
    # 測量 Tile
    start = time.time()
    _matrix.multiply_tile(A, B, tsize)
    time_tile = time.time() - start
    
    print(f"\nNaive time: {time_naive:.4f}s")
    print(f"Tile time: {time_tile:.4f}s")
    
    # 作業要求：Tile 執行時間必須小於 Naive 的 80%
    assert time_tile < (time_naive * 0.8), f"Tiling not fast enough! Tile: {time_tile:.4f}s, Naive: {time_naive:.4f}s"