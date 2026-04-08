import _matrix


def make_small_example():
    a = _matrix.Matrix(2, 3)
    b = _matrix.Matrix(3, 2)

    vals_a = [
        [1, 2, 3],
        [4, 5, 6],
    ]
    vals_b = [
        [7, 8],
        [9, 10],
        [11, 12],
    ]

    for i in range(2):
        for j in range(3):
            a[i, j] = vals_a[i][j]

    for i in range(3):
        for j in range(2):
            b[i, j] = vals_b[i][j]

    return a, b


def test_matrix_basic():
    m = _matrix.Matrix(3, 4)

    assert m.nrow == 3
    assert m.ncol == 4

    m[1, 2] = 9.5
    assert m[1, 2] == 9.5


def test_multiply_naive_small():
    a, b = make_small_example()
    c = _matrix.multiply_naive(a, b)

    assert c.nrow == 2
    assert c.ncol == 2

    assert c[0, 0] == 58
    assert c[0, 1] == 64
    assert c[1, 0] == 139
    assert c[1, 1] == 154


def test_multiply_tile_matches_naive():
    a, b = make_small_example()

    c1 = _matrix.multiply_naive(a, b)
    c2 = _matrix.multiply_tile(a, b, 2)
    c3 = _matrix.multiple_tile(a, b, 2)

    for i in range(c1.nrow):
        for j in range(c1.ncol):
            assert c1[i, j] == c2[i, j]
            assert c1[i, j] == c3[i, j]


def test_multiply_mkl_matches_naive():
    a, b = make_small_example()

    c1 = _matrix.multiply_naive(a, b)
    c2 = _matrix.multiply_mkl(a, b)

    for i in range(c1.nrow):
        for j in range(c1.ncol):
            assert c1[i, j] == c2[i, j]
