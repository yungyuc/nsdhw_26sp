import pytest
import math
import _vector

def test_zero_length_vector():
    with pytest.raises(ValueError):
        _vector.angle([0.0, 0.0], [1.0, 0.0])

def test_zero_angle():
    ans = _vector.angle([1.0, 0.0], [2.0, 0.0])
    assert math.isclose(ans, 0.0, abs_tol=1e-7)

def test_right_angle():
    ans = _vector.angle([1.0, 0.0], [0.0, 1.0])
    assert math.isclose(ans, math.pi / 2, abs_tol=1e-7)

def test_other_angle():
    ans = _vector.angle([1.0, 0.0], [1.0, 1.0])
    assert math.isclose(ans, math.pi / 4, abs_tol=1e-7)