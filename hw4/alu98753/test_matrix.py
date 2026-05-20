import pytest
import _matrix

@pytest.fixture
def mat1():
    return _matrix.Matrix(3,2)

@pytest.fixture
def mat2():
    return _matrix.Matrix(2,3)

# test constructer and getter
def test_constructer(mat1, mat2):
    assert mat1.nrow == 3
    assert mat1.ncol == 2
    assert mat2.nrow == 2
    assert mat2.ncol == 3

# test setter
def test_setter(mat1):
    mat1[0,0] = 1
    mat1[0,1] = 2
    mat1[1,0] = 3
    mat1[1,1] = 4
    mat1[2,0] = 5
    mat1[2,1] = 6
    assert mat1[0,0] == 1
    assert mat1[0,1] == 2
    assert mat1[1,0] == 3
    assert mat1[1,1] == 4
    assert mat1[2,0] == 5
    assert mat1[2,1] == 6

# test multiply_naive
def test_multiply_naive(mat1, mat2):
    mat1[0,0] = 1
    mat1[0,1] = 2
    mat1[1,0] = 3
    mat1[1,1] = 4
    mat1[2,0] = 5
    mat1[2,1] = 6
    mat2[0,0] = 7
    mat2[0,1] = 8
    mat2[0,2] = 9
    mat2[1,0] = 10
    mat2[1,1] = 11
    mat2[1,2] = 12
    result = _matrix.multiply_naive(mat1, mat2)
    assert result[0,0] == 27
    assert result[0,1] == 30
    assert result[0,2] == 33
    assert result[1,0] == 61
    assert result[1,1] == 68
    assert result[1,2] == 75
    assert result[2,0] == 95
    assert result[2,1] == 106
    assert result[2,2] == 117


# test multiply_mkl
def test_multiply_mkl(mat1, mat2):
    mat1[0,0] = 1
    mat1[0,1] = 2
    mat1[1,0] = 3
    mat1[1,1] = 4
    mat1[2,0] = 5
    mat1[2,1] = 6
    mat2[0,0] = 7
    mat2[0,1] = 8
    mat2[0,2] = 9
    mat2[1,0] = 10
    mat2[1,1] = 11
    mat2[1,2] = 12
    result = _matrix.multiply_mkl(mat1, mat2)
    assert result[0,0] == 27
    assert result[0,1] == 30
    assert result[0,2] == 33
    assert result[1,0] == 61
    assert result[1,1] == 68
    assert result[1,2] == 75
    assert result[2,0] == 95
    assert result[2,1] == 106
    assert result[2,2] == 117

# test mkl == naive
def test_mkl_equals_naive(mat1, mat2):
    mat1[0,0] = 1
    mat1[0,1] = 2
    mat1[1,0] = 3
    mat1[1,1] = 4
    mat1[2,0] = 5
    mat1[2,1] = 6
    mat2[0,0] = 7
    mat2[0,1] = 8
    mat2[0,2] = 9
    mat2[1,0] = 10
    mat2[1,1] = 11
    mat2[1,2] = 12
    result_mkl = _matrix.multiply_mkl(mat1, mat2)
    result_naive = _matrix.multiply_naive(mat1, mat2)
    assert result_mkl == result_naive

# test multiply_tile
def test_tile_equals_naive(mat1, mat2):
    mat1[0,0] = 1
    mat1[0,1] = 2
    mat1[1,0] = 3
    mat1[1,1] = 4
    mat1[2,0] = 5
    mat1[2,1] = 6
    mat2[0,0] = 7
    mat2[0,1] = 8
    mat2[0,2] = 9
    mat2[1,0] = 10
    mat2[1,1] = 11
    mat2[1,2] = 12
    result_tile = _matrix.multiply_tile(mat1, mat2, 2)
    result_naive = _matrix.multiply_naive(mat1, mat2)
    assert result_tile == result_naive
    assert result_tile[0,0] == 27
    assert result_tile[0,1] == 30
    assert result_tile[0,2] == 33
    assert result_tile[1,0] == 61
    assert result_tile[1,1] == 68
    assert result_tile[1,2] == 75
    assert result_tile[2,0] == 95
    assert result_tile[2,1] == 106
    assert result_tile[2,2] == 117
