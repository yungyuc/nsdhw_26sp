======================================================================
FastODE: A Lightweight High-Performance ODE Solver in C++ with
Python Interface
======================================================================

:Author: itsmalcomx
:Repository: https://github.com/itsmalcomx/FastODE

Problem to Solve
================

Ordinary differential equations (ODEs) are one of the most widely
used mathematical tools in science and engineering to describe how
systems change over time. Physical systems such as mechanic motion,
electrical circuits, and population models can be expressed as an
initial value problem (IVP):

.. code-block:: text

    dy/dt = f(t, y),  y(t0) = y0

where:

- ``y`` is the state vector representing the system,
- ``t`` represents time,
- ``f(t, y)`` is a function defining how the system evolves,
- ``y0`` is the initial condition.

The goal of an ODE solver is to compute the value of the system
state ``y`` at a later time ``t1 > t0`` given the initial state and
the function ``f(t, y)``.

This project implements two explicit Runge-Kutta methods for
solving non-stiff IVPs:

1. **Fourth-Order Runge-Kutta (RK4)**: A classical fixed-step
   method that evaluates ``f(t, y)`` four times per step using
   the Butcher tableau coefficients. It achieves local truncation
   error ``O(h^5)`` and global error ``O(h^4)``. Best suited for
   problems where a uniform step size is acceptable.

2. **Dormand-Prince (RK45)**: An embedded Runge-Kutta method that
   uses a 6-stage, 5th-order formula paired with a 4th-order
   error estimate to perform adaptive step-size control. The error
   estimate drives automatic step acceptance and rejection to meet
   user-specified tolerances ``rtol`` and ``atol``.

Existing libraries like SciPy have powerful ODE solvers but are
implemented in Python and not ideal for direct integration into C++
applications. FastODE aims to provide a lightweight solver that:

1. Provides a simple and transparent implementation of RK methods
   for educational use,
2. Allows direct integration into C++ applications,
3. Supports NumPy interoperability without unnecessary memory
   copying, and
4. Provides a clean Python interface similar to SciPy.

The project focuses on non-stiff ODEs, which are common in many
physical applications.

Prospective Users
=================

1. **Students and educators** in applied mathematics, physics, and
   engineering who want a transparent, readable RK4/RK45
   implementation they can study, modify, and extend.

2. **Scientific Python users** who need a drop-in replacement for
   ``scipy.integrate.solve_ivp`` with lower overhead for
   moderate-sized problems, especially when the right-hand side
   function is already implemented in C++.

3. **HPC developers** building simulation pipelines where the ODE
   solver is called millions of times (e.g. Monte Carlo trajectory
   ensembles). FastODE's zero-copy NumPy interface eliminates buffer
   allocation overhead on each call.

System Architecture
===================

FastODE is organized into three layers:

1. **C++ Core (libfastode)**: Contains solver classes
   ``ODESolverRK4`` and ``ODESolverRK45``. The solver stores the
   state vector in a contiguous array of doubles for efficient
   numerical computation. RK45 includes adaptive step-size control
   using error estimation between fourth- and fifth-order solutions.

2. **Python Bindings**: Implemented using pybind11, connecting the
   Python interface with C++ solver classes. The solution trajectory
   is exposed to Python using the NumPy buffer protocol, allowing
   NumPy arrays to access the underlying C++ memory without copying.

3. **Python Interface Module**: A thin Python module ``fastode``
   providing a user-friendly interface similar to SciPy. The solver
   returns a solution object containing:

   - ``t``: a 1D NumPy array of time points,
   - ``y``: a 2D NumPy array of shape ``(n_dims, n_steps)``,
   - ``n_steps``: total number of steps taken,
   - ``n_rejected``: number of rejected steps (RK45 only).

Workflow
--------

1. The user defines the function ``f(t, y)`` in Python.
2. The user provides the initial state ``y0``, time interval, and
   tolerances.
3. The Python interface passes the data to the C++ solver (without
   copying the NumPy buffer).
4. The C++ solver performs the RK4 or RK45 integration.
5. The computed trajectory is returned to Python as a NumPy array.
6. The user visualizes or analyzes the results.

Engineering Infrastructure
===========================

Build System
------------

CMake (3.18+) builds the C++ library and links against pybind11.
A ``pyproject.toml`` wraps CMake for Python packaging, allowing
installation with ``pip install -e .`` from the repository root.
The project targets C++17.

Version Control
---------------

Git with a two-branch workflow: ``main`` for stable tagged releases
and ``develop`` as the integration branch. Each feature or bug fix
lives on a short-lived branch and is merged via pull request after
CI passes. Commit messages follow the Conventional Commits
convention (``feat:``, ``fix:``, ``test:``, ``docs:``).

Testing
-------

C++ unit tests use Google Test (gtest) to verify the correctness of
the RK4 and RK45 solver kernels, including step computation and
error estimation logic. Validation against known analytical
solutions (exponential decay ``dy/dt = -ky``, simple harmonic
oscillator, Lotka-Volterra) is performed in Python using pytest,
which compares FastODE output against ``scipy.integrate.solve_ivp``
to within ``rtol=1e-5``. All tests must pass before any merge to
``main``.

Continuous Integration
----------------------

GitHub Actions runs on every push and pull request: compiles with
CMake, runs gtest, installs the Python package, and runs pytest.
The matrix covers Ubuntu (GCC) and macOS (Clang).

Documentation
-------------

Python API is documented with NumPy-style docstrings and built into
HTML with Sphinx + autodoc. C++ API is documented with Doxygen. A
Quick Start section in the README walks through the Lotka-Volterra
example end-to-end.

Schedule
========

+---------+--------+--------------------------------------------------+
| Phase   | Week   | Tasks                                            |
+=========+========+==================================================+
| Planning| W1     | Repo setup, CMake/pybind11, basic RK4,           |
|         | (3/9)  | GitHub Actions                                   |
+---------+--------+--------------------------------------------------+
|         | W2     | Vector RK4, unit tests, Python bindings          |
|         | (3/16) |                                                  |
+---------+--------+--------------------------------------------------+
|         | W3     | RK45 implementation, error estimation,           |
|         | (3/30) | SciPy tests                                      |
+---------+--------+--------------------------------------------------+
|         | W4     | Adaptive step-size control, refactoring          |
|         | (4/13) |                                                  |
+---------+--------+--------------------------------------------------+
|         | W5     | Python interface, fastode.solve(), test suite    |
|         | (4/27) |                                                  |
+---------+--------+--------------------------------------------------+
|         | W6     | Validation utility, dense output, Sphinx docs    |
|         | (5/11) |                                                  |
+---------+--------+--------------------------------------------------+
|         | W7     | Performance benchmarking vs SciPy                |
|         | (5/25) |                                                  |
+---------+--------+--------------------------------------------------+
|         | W8     | Buffer week, event detection, demo notebooks     |
|         | (6/8)  |                                                  |
+---------+--------+--------------------------------------------------+
