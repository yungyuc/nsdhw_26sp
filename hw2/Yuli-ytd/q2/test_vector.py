import _vector
import math
import pytest

def test_zero_vector():
    with pytest.raises(RuntimeError) as excinfo:
        _vector.calculate_angle(0, 0, 1, 1)

    assert "Zero-length vector is invalid." in str(excinfo.value)

def test_zero_angle():
    result =  _vector.calculate_angle(1, 1, 1, 1)
    assert result == pytest.approx(0.0, abs=1e-7)

def test_right_angle():
    result = _vector.calculate_angle(1, 0, 0, 1)
    assert result == pytest.approx(math.pi / 2)

def test_other_angle():
    result = _vector.calculate_angle(1, 0, 1, 1)
    assert result == pytest.approx(math.pi / 4)