import math
import pytest
import _vector as angle  # the pybind11 module built via make


class TestAngleBetween:
    """Tests for angle.angle_between(x1, y1, x2, y2)."""

    def test_same_direction(self):
        """Parallel vectors pointing the same way → 0 radians."""
        assert angle.angle_between(1, 0, 2, 0) == pytest.approx(0.0)

    def test_opposite_direction(self):
        """Anti-parallel vectors → π radians."""
        assert angle.angle_between(1, 0, -1, 0) == pytest.approx(math.pi)

    def test_perpendicular(self):
        """Orthogonal vectors → π/2 radians."""
        assert angle.angle_between(1, 0, 0, 1) == pytest.approx(math.pi / 2)

    def test_45_degrees(self):
        """45-degree angle → π/4 radians."""
        assert angle.angle_between(1, 0, 1, 1) == pytest.approx(math.pi / 4)

    def test_symmetric(self):
        """angle(a,b) == angle(b,a)."""
        a = angle.angle_between(3, 4, 1, 0)
        b = angle.angle_between(1, 0, 3, 4)
        assert a == pytest.approx(b)

    def test_scaling_invariant(self):
        """Scaling a vector should not change the angle."""
        a = angle.angle_between(1, 0, 1, 1)
        b = angle.angle_between(100, 0, 5, 5)
        assert a == pytest.approx(b)

    def test_negative_components(self):
        """Vectors in the third/fourth quadrant."""
        # Vector (-1,0) and (0,-1) are still perpendicular
        assert angle.angle_between(-1, 0, 0, -1) == pytest.approx(math.pi / 2)

    def test_zero_vector_raises(self):
        """A zero-length vector should raise ValueError (or similar)."""
        with pytest.raises(Exception):
            angle.angle_between(0, 0, 1, 0)

    def test_result_in_range(self):
        """Result must always be in [0, π]."""
        pairs = [(1, 2, 3, 4), (-1, -1, 2, 3), (0, 1, -1, 0), (5, -5, -3, 3)]
        for x1, y1, x2, y2 in pairs:
            result = angle.angle_between(x1, y1, x2, y2)
            assert 0.0 <= result <= math.pi + 1e-9, (
                f"Out of range for ({x1},{y1}),({x2},{y2}): {result}"
            )