import _vector
import math
import pytest


def test_zero_vector():
    # first vector is zero vector (0,0)
    with pytest.raises(ValueError):
        _vector.angle(0, 0, 1, 0)

    # second vector is zero vector (0,0)
    with pytest.raises(ValueError):
        _vector.angle(1, 0, 0, 0)

    # both are zero vectors
    with pytest.raises(ValueError):
        _vector.angle(0, 0, 0, 0)


def test_zero_angle():
    result = _vector.angle(1, 0, 2, 0)
    assert result == pytest.approx(0.0)

    result = _vector.angle(3, 3, 1, 1)
    assert result == pytest.approx(0.0)


def test_right_angle():
    result = _vector.angle(1, 0, 0, 1)
    assert result == pytest.approx(math.pi / 2)


def test_45_degree_angle():
    result = _vector.angle(1, 0, 1, 1)
    assert result == pytest.approx(math.pi / 4)


def test_180_degree_angle():
    result = _vector.angle(1, 0, -1, 0)
    assert result == pytest.approx(math.pi)
