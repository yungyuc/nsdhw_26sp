=====================================================================
Proposal: FastGA - A High-Performance Hybrid Genetic Algorithm Engine
=====================================================================

Basic Information
=================

**Author:** jylin34
**Repository:** https://github.com/jylin34/FastGA

FastGA is a high-performance C++ genetic algorithm (GA) library with 
Python bindings, specifically optimized for global optimization problems 
represented by **1D double-precision vectors**. 

The project bridges the gap between Python's development flexibility and 
C++'s raw execution speed. It allows users to define complex, vectorized 
fitness functions in Python while offloading the memory-intensive 
evolutionary loops and genetic operators to a multi-threaded C++ engine.

Problem to Solve
================

Genetic Algorithms (GA) are a class of heurstic search and optimization
techniques inspired by the process of natural selection. They are used to solve
complex optimization problems by evolving a population of candidate solutions
using operators such as selection, crossover, and mutation. As a stochastic,
population-based approach, GA is particularly effictive for navigating
high-dimensional or non-convex search spaces where traditional gradient-based
methods may fail.

Specifically, FastGA focuses on optimization problems where candidate solutions 
are represented as **1D vectors of double-precision floating-point numbers**. 
While this representation is versatile—covering everything from financial 
portfolio weights to physical control parameters—it introduces significant 
computational challenges when implemented in pure Python:

1. **Overhead of 1D Vector Processing**:
   As the dimension of the 1D vector (chromosome) increases, performing element-
   wise genetic operations in Python becomes extremely slow due to interpreter 
   loop overhead and dynamic type-checking.

2. **The "Flexibility vs. Performance" Trade-off**:
   Users often require **custom, user-defined fitness functions** to evaluate 
   their specific problems. In existing libraries like PyGAD, calling these 
   custom Python functions within nested C-style loops creates a massive 
   bottleneck. There is a lack of tools that provide the flexibility of 
   Python-defined logic with the execution speed of a compiled C++ engine.

3. **Memory and Parallelism Inefficiency**:
   Large-scale 1D vector populations require contiguous memory layouts to 
   benefit from CPU cache prefetching and SIMD instructions. Furthermore, 
   Python's Global Interpreter Lock (GIL) prevents true multi-core utilization 
   during the fitness evaluation of these vectors.

FastGA aims to solve these problems by providing a high-performance C++ core 
that manages contiguous 1D arrays while offering a seamless "Batch Callback" 
interface for user-defined fitness functions in Python.

Prospective Users
=================

FastGA is desgined for users who require both the rapid prototyping capabilities
of Python and the computational performance of C++. The potential user includes:

1. **Scientific Researchers**
2. **Quantitative Financial Analysts**
3. **Machined Learning Engineer**
4. **Students and Educators**

System Architecture
===================

FastGA adopts a three-tier hybrid architecture to maximize throughput while 
maintaining user-defined flexibility in Python. All performance-
critical operators, such as crossover and mutation, are implemented in
the C++ core. This ensures the entire population is processed in a single
batch call, avoiding the overhead of Python-level iteration.

1. **Python Control Layer**: 
   Users define the optimization problem and the fitness logic. To optimize 
   performance, the system requires a **Vectorized Fitness Function**. The 
   engine passes the entire 1D vector population to Python as a single 2D 
   NumPy array once per generation.

2. **Interoperability Layer (pybind11)**: 
   Uses the NumPy Buffer Protocol to achieve **Zero-copy** data sharing. The 
   C++ engine shares its internal memory address directly with Python's 
   NumPy, allowing both layers to operate on the same raw data without 
   expensive serialization.

3. **C++ High-Performance Engine**:

   * **Data Representation**: Stores the population in a **strictly 
     contiguous 1D array** (Row-major layout) to ensure spatial locality.
   * **Parallelized Operators**: Crossover and mutation are parallelized 
     using **OpenMP**, allowing simultaneous modification of individuals 
     across all available CPU cores.
   * **GIL Management**: The engine explicitly **releases the GIL** during 
     C++ reproduction phases, ensuring that the computational core does not 
     block the Python interpreter.

Workflow:
---------
1. Initialize a stable, fixed-size, contiguous memory buffer in C++.
2. **Reproduction Phase**: C++ uses OpenMP to perform mutation/crossover 
   on all 1D vectors in parallel.
3. **Evaluation Phase**: C++ maps the buffer to a NumPy array and makes a 
   **single call** to the Python fitness function.
4. **Selection Phase**: C++ performs survival selection based on the 
   returned scores and prepares the next generation.

API Description
===============

FastGA allows users to define both the chromosome dimension and the 
optimization goal in Python.

Example: Solving the Rosenbrock Function
----------------------------------------

The Rosenbrock function is a non-convex function where the global minimum 
is inside a long, narrow, parabolic-shaped flat valley. 

$$f(x) = \sum_{i=1}^{n-1} [100(x_{i+1} - x_i^2)^2 + (1 - x_i)^2]$$

.. code-block:: python

   import fastga
   import numpy as np

   # 1. Define a Vectorized Fitness Function
   # 'population' is a NumPy view of the C++ memory (shape: N x Dim)
   def rosenbrock_fitness(population):
       x = population[:, :-1]
       x_next = population[:, 1:]
       # High-speed vectorized calculation using NumPy
       scores = np.sum(100.0*(x_next - x**2.0)**2.0 + (1.0 - x)**2.0, axis=1)
       return -scores  # Negative because GA maximizes fitness

   # 2. Setup: 1000 individuals, each a 50-dimensional 1D vector
   pop = fastga.Population(pop_size=1000, genome_size=50)

   # 3. Configure Engine
   engine = fastga.GAEngine(mutation_rate=0.02, crossover_rate=0.8)
   engine.set_batch_fitness(rosenbrock_fitness)

   # 4. Evolve
   engine.evolve(pop, generations=2000)

   # 5. Result
   print(f"Best Solution found: {pop.get_best()}")

   # In this case, traditional approach will use up to 2000 * 1000 for loops,
   # means that fitness function will be called 2000000 times.

Core Interface:
---------------

* ``fastga.Population(size, dim)``: Allocates contiguous memory for ``size`` 
  individuals with ``dim`` variables (1D vector size).
* ``GAEngine.set_batch_fitness(func)``: Registers the Python callback for 
  vectorized evaluation.
* ``GAEngine.evolve(pop, gens)``: Triggers the C++ loop, OpenMP threads, 
  and GIL management.

Engineering Infrastructure
==========================

* **Automatic Build System**: FastGA uses **CMake** for C++ compilation 
  and **pybind11** for Python wrapping. 
* **Version Control**: Managed via **Git/GitHub** following a feature-branch 
  and Pull Request workflow.
* **Testing Framework**: **pytest** is used to verify API correctness, 
  zero-copy integrity, and convergence on numerical benchmarks.

Schedule
========

* **Week 1 (03/16 to 03/22)**: Implement the CMake build system and initial
  ``pytest`` framework. Define a stable, fixed-size C++ ``Population`` 
  buffer and expose it via Buffer Protocol. 
* **Week 2 (03/23 to 03/29)**: Core C++ Operators. Develop the initial single-
  threaded kernels for crossover and mutation. Ensure these operators 
  interact correctly with the raw double-precision memory buffers.
* **Week 3 (03/30 to 04/05)**: Integration and Batch Evaluation. Implement
  the bridge for user-defined Python fitness functions. Verify that the
  Python-side NumPy views can safely process the C++ memory buffers. 
* **Week 4 (04/06 to 04/12)**: Evolutionary Loop. Implement the selection 
  logic and orchestrate the main GA cycle in C++. Verify end-to-end 
  functionality (Initialization -> Evaluation -> Reproduction) from Python.
* **Week 5 (04/13 to 04/19)**: Concurrency & OpenMP. Integrate **OpenMP** 
  into the reproduction kernels. Implement GIL management (releasing GIL 
  during C++ phases) to enable stable multi-core execution of GA operators.
* **Week 6 (04/20 to 04/26)**: Numerical Application. Implement the 
  **Rosenbrock function** and other benchmarks in Python. Verify the engine's 
  convergence and stability under parallel execution modes.
* **Week 7 (04/27 to 05/03)**: Robustness & Refinement. Conduct tests with 
  large-scale populations and high-dimensional vectors. Refine the Python 
  API and internal memory management based on profiling results.
* **Week 8 (05/04 to 05/10)**: Documentation and Final Benchmarking. Perform 
  comparative experiments against PyGAD. Finalize the Prepare the presentation
  and demo materials.

References
==========

1. **Course Website**: 
   https://yyc.solvcon.net/en/latest/nsd/index.html

2. **pybind11 Documentation**: 
   https://pybind11.readthedocs.io/

3. **PyGAD (Genetic Algorithm in Python)**: 
   https://github.com/ahmedfgad/GeneticAlgorithmPython

4. **NumPy Buffer Protocol**: 
   https://numpy.org/doc/stable/reference/arrays.interface.html

5. **OpenMP Specification**: 
   https://www.openmp.org/specifications/
