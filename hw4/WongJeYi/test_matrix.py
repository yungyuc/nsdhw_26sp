import _matrix
import unittest

class TestMatrix(unittest.TestCase):
    
    def assert_matrix_equal(self,m_result, m_expected, msg):
            self.assertEqual(m_result.nrow, m_expected.nrow, f"{msg}: Row mismatch")
            self.assertEqual(m_result.ncol, m_expected.ncol, f"{msg}: Col mismatch")
            for i in range(m_result.nrow):
                for j in range(m_result.ncol):
                    self.assertEqual(m_result[i, j], m_expected[i, j], f"{msg} at index [{i},{j}]")
    def test_zero(self):
        """Test that the matrix is created and initialized to 0."""
        mat = _matrix.Matrix(10, 20)
        self.assertEqual(mat.nrow, 10)
        self.assertEqual(mat.ncol, 20)
        self.assertEqual(mat[0, 0], 0.0)
        self.assertEqual(mat[9, 19], 0.0)

    def test_multiplication_correctness(self):
        """Test that all three multiplication methods return the correct math."""
        # Matrix 1 (2x3)
        # [1, 2]
        # [3, 4]
        m1 = _matrix.Matrix(2, 2)
        m1[0, 0] = 1; m1[0, 1] = 2
        m1[1, 0] = 3; m1[1, 1] = 4

        # Matrix 2 (2x1)
        # [1]
        # [2]
        m2 = _matrix.Matrix(2, 1)
        m2[0, 0] = 1
        m2[1, 0] = 2

        # Expected Result (2x1)
        # [5]
        # [11]
        expected = _matrix.Matrix(2, 1)
        expected[0, 0] = 5;  expected[1, 0] = 11

        # Calculate using all three methods
        ret_naive = _matrix.multiply_naive(m1, m2)
        ret_tile = _matrix.multiply_tile(m1, m2, 2)  # Testing with tsize=2
        ret_mkl = _matrix.multiply_mkl(m1, m2)

        # Assert all results match the expected math
        self.assert_matrix_equal(ret_naive, expected, "Naive multiplication failed!")
        self.assert_matrix_equal(ret_tile, expected, "Tiled multiplication failed!")
        self.assert_matrix_equal(ret_mkl, expected, "MKL multiplication failed!")

if __name__ == '__main__':
    unittest.main()