import _matrix
import pytest

def test_basic_creation():
    mat = _matrix.Matrix(10, 10)
    assert mat.nrow == 10
    assert mat.ncol == 10
    assert mat[0, 0]