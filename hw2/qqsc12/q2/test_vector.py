import _vector
import math
import pytest

def test_zero_angle():
    # 同向：角度 0
    assert math.isclose(_vector.angle(1, 0, 1, 0), 0.0, abs_tol=1e-6)

def test_right_angle():
    # 垂直：角度 pi/2
    assert math.isclose(_vector.angle(1, 0, 0, 1), math.pi / 2, abs_tol=1e-6)

def test_zero_vector():
    # 老師要求的 Invalid input 測試
    assert _vector.angle(0, 0, 1, 1) == 0.0

def test_opposite_angle():
    # 反向：角度 pi (180度)
    assert math.isclose(_vector.angle(1, 0, -1, 0), math.pi, abs_tol=1e-6)