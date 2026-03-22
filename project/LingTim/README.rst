# Reimplement NumPy Matrix Operations

## URL
https://github.com/LingTim/Reimplement-NumPy-Matrix-Operations

## Author
GitHub Account: LingTim

## Motivation
Since native Python is inherently slow for numerical computations, developers
rely heavily on NumPy for acceleration. While NumPy's backend is
traditionally built with C and Fortran, modern C++ has become the mainstream
standard for system software.
Therefore, this project aims to simulate the underlying logic of NumPy's
matrix operations using purely C++.

## Operations and Routines
I will implement the following core operations as C++ extensions for Python:

1. Matrix-matrix addition and subtraction.
2. Matrix-matrix multiplication.
3. Matrix-vector multiplication.
4. Matrix-scalar multiplication.
5. Matrix-scalar division.

**Optimization & Interface:**
* All operations will first be implemented in standard C++ (naive baseline)
to verify correctness. Subsequently, the **AVX instruction set** will be
explicitly utilized to vectorize and accelerate the computations.
* **Pybind11** will be used to wrap and compile these C++ functions into
`.pyd` dynamic libraries, making them directly callable within Python.

## Performance Goals
Instead of attempting to beat the highly optimized production-level NumPy,
the goal of this project is to benchmark and analyze the performance
differences between pure Python implementations, the naive C++ baseline,
and the AVX-optimized C++ versions.

## Schedule
* **Week 6:** Implement matrix-scalar multiplication.
* **Week 7:** Implement matrix-scalar division.
* **Week 9:** Implement matrix-vector multiplication.
* **Week 10:** Implement matrix-matrix addition and subtraction.
* **Week 11:** Implement matrix-matrix multiplication and AVX optimizations.
* **Week 12:** Benchmark, compare performance differences, and finalize the report.

