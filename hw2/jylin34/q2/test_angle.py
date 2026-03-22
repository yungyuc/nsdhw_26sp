import pytest
import math
import _vector  # Import the C++ module

def test_right_angle():
    """
    Tests the angle between two orthogonal vectors, 90 degrees (π/2 radians).
    """
    angle = _vector.calculate_angle(1, 0, 0, 1)
    assert angle == pytest.approx(math.pi / 2)

def test_zero_angle():
    """
    Tests the angle between two vectors pointing in the same direction, 0 degrees.
    """
    angle = _vector.calculate_angle(1, 1, 2, 2)
    assert angle == pytest.approx(0.0, abs=1e-7)

def test_straight_angle():
    """
    Tests the angle between two vectors pointing in opposite directions, 180 degrees (π radians).
    """
    angle = _vector.calculate_angle(1, 0, -1, 0)
    assert angle == pytest.approx(math.pi)

def test_45_degree_angle():
    """
    Tests 45-degree angle (π/4 radians).
    """
    angle = _vector.calculate_angle(1, 0, 1, 1)
    assert angle == pytest.approx(math.pi / 4)

def test_zero_vector_input():
    """
    Tests if function raise ValueError as expected when a zero vector is given.
    """
    with pytest.raises(ValueError, match="Input vector cannot be a zero vector."):
        _vector.calculate_angle(1, 1, 0, 0)
    
    with pytest.raises(ValueError, match="Input vector cannot be a zero vector."):
        _vector.calculate_angle(0, 0, 1, 1)

def test_order_invariance():
    """
    Tests that swapping the order of vectors should not affect the final angle calculation.
    """
    angle1 = _vector.calculate_angle(1, 2, 3, 4)
    angle2 = _vector.calculate_angle(3, 4, 1, 2)
    assert angle1 == pytest.approx(angle2)
