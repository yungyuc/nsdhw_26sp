import pytest
import _matrix


#  the helpers i used 

def make_matrices(size):
    """Return (mat1, mat2, mat3_zeros) filled like validate.py."""
    mat1 = _matrix.Matrix(size, size)
    mat2 = _matrix.Matrix(size, size)
    mat3 = _matrix.Matrix(size, size)
    for it in range(size):
        for jt in range(size):
            mat1[it, jt] = it * size + jt + 1
            mat2[it, jt] = it * size + jt + 1
            mat3[it, jt] = 0
    return mat1, mat2, mat3


#  my matrix class 

class TestMatrix:

    def test_basic(self):
        """Mirror validate.py GradingTest.test_basic exactly."""
        size = 100
        mat1, mat2, mat3 = make_matrices(size)

        assert mat1.nrow == size
        assert mat1.ncol == size
        assert mat2.nrow == size
        assert mat2.ncol == size
        assert mat3.nrow == size
        assert mat3.ncol == size

        assert mat1[0, 1] == 2
        assert mat1[1, 1] == size + 2
        assert mat1[1, size-1] == size * 2
        assert mat1[size-1, size-1] == size * size

        for i in range(mat1.nrow):
            for j in range(mat1.ncol):
                assert mat1[i, j] != 0
                assert mat1[i, j] == mat2[i, j]
                assert mat3[i, j] == 0

        assert mat1 == mat2
        assert mat1 is not mat2

    def test_shape(self):
        M = _matrix.Matrix(4, 7)
        assert M.nrow == 4
        assert M.ncol == 7

    def test_zero_init(self):
        M = _matrix.Matrix(3, 3)
        for i in range(3):
            for j in range(3):
                assert M[i, j] == 0.0

    def test_setget(self):
        M = _matrix.Matrix(5, 5)
        M[2, 4] = 3.14
        assert abs(M[2, 4] - 3.14) < 1e-12

    def test_equality(self):
        A = _matrix.Matrix(3, 3)
        B = _matrix.Matrix(3, 3)
        A[1, 2] = 9.0
        B[1, 2] = 9.0
        assert A == B
        B[0, 0] = 1.0
        assert not (A == B)




class TestNaive:

    def test_match_mkl(self):
        """Mirror validate.py GradingTest.test_match_naive_mkl."""
        size = 100
        mat1, mat2, _ = make_matrices(size)
        ret_naive = _matrix.multiply_naive(mat1, mat2)
        ret_mkl   = _matrix.multiply_mkl(mat1, mat2)

        assert ret_naive.nrow == size
        assert ret_naive.ncol == size

        for i in range(ret_naive.nrow):
            for j in range(ret_naive.ncol):
                assert ret_naive[i, j] != mat1[i, j]
                assert ret_naive[i, j] == pytest.approx(ret_mkl[i, j], rel=1e-6)

    def test_zero(self):
        """Mirror validate.py GradingTest.test_zero."""
        size = 100
        mat1, _, mat3 = make_matrices(size)
        ret_naive = _matrix.multiply_naive(mat1, mat3)
        ret_mkl   = _matrix.multiply_mkl(mat1, mat3)
        for i in range(ret_naive.nrow):
            for j in range(ret_naive.ncol):
                assert ret_naive[i, j] == 0.0
                assert ret_mkl[i, j]   == 0.0

    def test_2x2_known(self):
      
        A = _matrix.Matrix(2, 2)
        B = _matrix.Matrix(2, 2)
        A[0,0]=1; A[0,1]=2; A[1,0]=3; A[1,1]=4
        B[0,0]=5; B[0,1]=6; B[1,0]=7; B[1,1]=8
        C = _matrix.multiply_naive(A, B)
        assert C[0,0] == pytest.approx(19)
        assert C[0,1] == pytest.approx(22)
        assert C[1,0] == pytest.approx(43)
        assert C[1,1] == pytest.approx(50)

    def test_dimension_mismatch(self):
        A = _matrix.Matrix(3, 4)
        B = _matrix.Matrix(5, 3)
        with pytest.raises(Exception):
            _matrix.multiply_naive(A, B)




class TestTile:

    @pytest.mark.parametrize("tsize", [16, 17, 19, 32, 64])
    def test_match_mkl(self, tsize):
        """Tile output must match MKL for all grader-tested tile sizes."""
        size = 100
        mat1, mat2, _ = make_matrices(size)
        ret_tile = _matrix.multiply_tile(mat1, mat2, tsize)
        ret_mkl  = _matrix.multiply_mkl(mat1, mat2)

        assert ret_tile.nrow == size
        assert ret_tile.ncol == size
        for i in range(ret_tile.nrow):
            for j in range(ret_tile.ncol):
                assert ret_tile[i, j] == pytest.approx(ret_mkl[i, j], rel=1e-6)

    def test_zero(self):
        size = 100
        mat1, _, mat3 = make_matrices(size)
        ret = _matrix.multiply_tile(mat1, mat3, 16)
        for i in range(ret.nrow):
            for j in range(ret.ncol):
                assert ret[i, j] == 0.0

    def test_2x2_known(self):
        A = _matrix.Matrix(2, 2)
        B = _matrix.Matrix(2, 2)
        A[0,0]=1; A[0,1]=2; A[1,0]=3; A[1,1]=4
        B[0,0]=5; B[0,1]=6; B[1,0]=7; B[1,1]=8
        C = _matrix.multiply_tile(A, B, 16)
        assert C[0,0] == pytest.approx(19)
        assert C[0,1] == pytest.approx(22)
        assert C[1,0] == pytest.approx(43)
        assert C[1,1] == pytest.approx(50)

    def test_non_square_input(self):
        # 2x3 @ 3x4 = 2x4
        A = _matrix.Matrix(2, 3)
        B = _matrix.Matrix(3, 4)
        for i in range(2):
            for j in range(3):
                A[i, j] = float(i*3 + j + 1)
        for i in range(3):
            for j in range(4):
                B[i, j] = float(i*4 + j + 1)
        ref = _matrix.multiply_mkl(A, B)
        res = _matrix.multiply_tile(A, B, 2)
        assert res.nrow == 2
        assert res.ncol == 4
        for i in range(2):
            for j in range(4):
                assert res[i, j] == pytest.approx(ref[i, j], rel=1e-6)

    def test_dimension_mismatch(self):
        A = _matrix.Matrix(3, 4)
        B = _matrix.Matrix(5, 3)
        with pytest.raises(Exception):
            _matrix.multiply_tile(A, B, 16)

    def test_tsize_zero_raises(self):
        mat1, mat2, _ = make_matrices(4)
        with pytest.raises(Exception):
            _matrix.multiply_tile(mat1, mat2, 0)




class TestMkl:

    def test_2x2_known(self):
        A = _matrix.Matrix(2, 2)
        B = _matrix.Matrix(2, 2)
        A[0,0]=1; A[0,1]=2; A[1,0]=3; A[1,1]=4
        B[0,0]=5; B[0,1]=6; B[1,0]=7; B[1,1]=8
        C = _matrix.multiply_mkl(A, B)
        assert C[0,0] == pytest.approx(19)
        assert C[0,1] == pytest.approx(22)
        assert C[1,0] == pytest.approx(43)
        assert C[1,1] == pytest.approx(50)

    def test_dimension_mismatch(self):
        A = _matrix.Matrix(3, 4)
        B = _matrix.Matrix(5, 3)
        with pytest.raises(Exception):
            _matrix.multiply_mkl(A, B)




class TestConsistency:

    @pytest.mark.parametrize("tsize", [16, 17, 19])
    def test_all_agree(self, tsize):
        """naive, tile, mkl must all match on a 200x200 random-ish matrix."""
        size = 200
        mat1, mat2, _ = make_matrices(size)
        ret_naive = _matrix.multiply_naive(mat1, mat2)
        ret_tile  = _matrix.multiply_tile (mat1, mat2, tsize)
        ret_mkl   = _matrix.multiply_mkl  (mat1, mat2)
        for i in range(0, size, 20):   # spot-check every 20th row
            for j in range(0, size, 20):
                assert ret_naive[i, j] == pytest.approx(ret_mkl[i, j],   rel=1e-6)
                assert ret_tile [i, j] == pytest.approx(ret_mkl[i, j],   rel=1e-6)