import math
import pytest
import _vector


def test_zero_angle():
    ans = _vector.angle_between((3.0, 3.0), (6.0, 6.0))
    assert math.isclose(ans, 0.0, rel_tol=1e-12, abs_tol=1e-12)


def test_right_angle():
    ans = _vector.angle_between((2.0, 5.0), (-5.0, 2.0))
    assert math.isclose(ans, math.pi / 2.0, rel_tol=1e-12, abs_tol=1e-12)


def test_other_angle():
    ans = _vector.angle_between((1.0, 0.0), (1.0, 1.0))
    assert math.isclose(ans, math.pi / 4.0, rel_tol=1e-12, abs_tol=1e-12)


def test_zero_length_vector():
    with pytest.raises(Exception):
        _vector.angle_between((0.0, 0.0), (4.0, 7.0))
