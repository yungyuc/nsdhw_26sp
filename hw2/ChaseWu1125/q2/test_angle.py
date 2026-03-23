import pytest
import math
import _vector

def test_zero_angle():
    assert _vector.angle(1, 0, 5, 0) == pytest.approx(0)

def test_ninenty_degree_angle():
    assert _vector.angle(1, 0, 0, 5) == pytest.approx(math.pi / 2)

def test_one_eighty_degree_angle():
    assert _vector.angle(1, 0, -1, 0) == pytest.approx(math.pi)

def test_fourty_five_degree_angle():
    assert _vector.angle(1, 0, 1, 1) == pytest.approx(math.pi / 4)

def test_zero_length_vector():
    with pytest.raises(ValueError):
        _vector.angle(0, 0, 1, 1)