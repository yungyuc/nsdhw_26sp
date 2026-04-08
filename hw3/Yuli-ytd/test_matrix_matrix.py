import _matrix as _mat
import pytest
import timeit

# basic functionality
def test_matrix_properties():
    
    mat = _mat.Matrix(5, 3)
    assert mat.nrow == 5
    assert mat.ncol == 3
    assert mat[0, 0] == 0.0

    mat[0, 0] = 3.5
    assert mat[0, 0] == pytest.approx(3.5)

# multipliable test
def test_multipliable():

    mat1 = _mat.Matrix(20, 10)
    mat2 = _mat.Matrix(20, 10)

    # multipliable
    with pytest.raises(IndexError) as excinfo:
        _mat.multiply_naive(mat1, mat2)
    
    assert "Matrices aren't multipliable" in str(excinfo.value)

# calculation correctness
def test_multiplication():
    
    mat1 = _mat.Matrix(10, 20)
    mat2 = _mat.Matrix(20, 15)

    for i in range(mat1.nrow):
        for j in range(mat1.ncol):
            mat1[i, j] = i + j
    for i in range(mat2.nrow):
        for j in range(mat2.ncol):
            mat2[i, j] = i * j 

    result_naive = _mat.multiply_naive(mat1, mat2)
    result_tile_8 = _mat.multiply_tile(mat1, mat2, 8)
    result_tile_16 = _mat.multiply_tile(mat1, mat2, 16)
    result_mkl = _mat.multiply_mkl(mat1, mat2)

    # naive vs. mkl vs. tile
    for i in range(result_mkl.nrow):
        for j in range(result_mkl.ncol):
            tar = result_mkl[i, j]
            assert result_naive[i, j] == pytest.approx(tar)
            assert result_tile_8[i, j] == pytest.approx(tar)
            assert result_tile_16[i, j] == pytest.approx(tar)

# performance
def test_performance():

    size = 1000
    mat1 = _mat.Matrix(size, size)
    mat2 = _mat.Matrix(size, size)

    for i in range(size):
        for j in range(size):
            mat1[i, j] = i + j
            mat2[i, j] = i * j
    
    def run_naive():
        _mat.multiply_naive(mat1, mat2)
    def run_mkl():
        _mat.multiply_mkl(mat1, mat2)
    
    t_naive = min(timeit.repeat(run_naive, repeat=5, number=1))
    t_mkl = min(timeit.repeat(run_mkl, repeat=5, number=1))

    # print(f'Naive time: {t_naive:.4f} s\n')
    # print(f'Tile time: {t_tile:.4f} s\n')
    # print(f'Speedup: {t_naive / t_tile:.2f} x\n')
    # print(f'Tile is {t_tile / t_naive:.2f} of naive time\n')

    assert (t_mkl / t_naive < 0.8)