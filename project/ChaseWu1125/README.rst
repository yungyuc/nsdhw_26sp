=================
FastMCPricer
=================


Basic Information
=================
A high-performance option pricing model utilizing parallelized Monte 
Carlo simulations in C++ with a Python wrapper.
GitHub Repository: https://github.com/ChaseWu1125/FastMCPricer


Problem to Solve
================

This project aims to address the performance bottleneck in pricing 
complex financial derivatives using numerical methods.

1. **The Field:**
   In quantitative finance, while standard European options have
   closed-form solutions (e.g., the Black-Scholes formula), many exotic 
   options (such as Asian or Barrier options) that are heavily traded
   in the market do not. 
   
   Financial engineers, quantitative analysts, and traders must rely on 
   numerical methods to estimate their fair theoretical value and risk 
   metrics.

2. **The Mathematics:**
   Under the risk-neutral pricing framework, the underlying asset price 
   is standardly modeled to follow Geometric Brownian Motion (GBM). The 
   stochastic differential equation is given by:

   .. math::

      dS_t = r S_t dt + \sigma S_t dW_t

   where :math:`r` is the risk-free interest rate, :math:`\sigma` is the 
   volatility, and :math:`W_t` is a Wiener process. The fair price of an 
   option is the expected value of its future payoff discounted at the 
   risk-free rate.

3. **The Algorithm and Numerical Method:**
   Monte Carlo (MC) simulation is a robust numerical method to calculate 
   this expected payoff by simulating a massive number of random asset 
   price paths. 
   
   However, based on the Law of Large Numbers, the standard error of an 
   MC simulation decreases at a rate of :math:`O(1/\sqrt{N})`. 
   
   This implies that increasing the pricing precision by a factor of 10 
   requires 100 times more computational effort.

   A pure Python implementation iterating through millions of paths is 
   impractically slow due to the Global Interpreter Lock (GIL) and 
   interpretation overhead. 
   
   This project solves this computation constraint by implementing the 
   core MC simulation engine in C++, utilizing multi-threading to 
   parallelize the path generations across multiple CPU cores, and 
   wrapping it with a user-friendly Python interface.


Prospective Users
=================

The prospective users of this software include financial engineering 
students, quantitative analysts, and algorithmic traders. 

Users will utilize the software primarily through its Python interface 
(e.g., in Jupyter Notebooks or Python scripts).

 They can define option parameters (strike price, maturity, etc.), 
 market conditions (spot price, volatility, risk-free rate), and 
 hardware configurations (number of simulation paths and CPU threads). 
 
 The system will return the estimated option price and the computational
 time, allowing users to efficiently analyze pricing convergence and 
 hardware scaling performance without needing to interact with the 
 underlying C++ code.

System Architecture
===================

The system follows a modular architecture designed to decouple the 
mathematical models from the numerical engines, ensuring both high 
performance and flexibility. The architecture consists of three 
main layers:

1. **Frontend (Python):** Provides a user-friendly interface. 
   It handles user inputs, validates parameters, and visualizes 
   the results (e.g., pricing convergence graphs and performance 
   benchmarks).
   
2. **Binding Layer (pybind11):** Acts as the bridge between Python and 
   C++. It securely and efficiently transfers Python objects into C++ 
   data structures and returns the computed results back to the Python 
   environment.

3. **Backend (C++):** The core computational engine, optimized for 
   speed and memory efficiency. It is divided into several independent 
   modules:
   
   * **Data Structure:** 2D asset paths and random number matrices will 
     be stored in a flattened 1D ``std::vector<double>``. This contiguous 
     memory layout maximizes cache locality and avoids the overhead of 
     nested vectors.
   * **Option Module:** An object-oriented hierarchy defining the 
     financial instruments. A base ``Option`` class will be extended 
     by ``EuropeanOption``, ``AsianOption``, and ``BarrierOption``.
   * **Random Number Generator (RNG) Module:** Generates standard 
     normal random variables. To prevent race conditions in parallel 
     execution, thread-safe RNGs (e.g., using independent seeds for 
     ``std::mt19937`` per thread) will be implemented.
   * **Pricing Engine:** Includes an ``AnalyticalPricer`` (for standard 
     Black-Scholes formulas as a baseline) and a ``MonteCarloPricer``. 
     The ``MonteCarloPricer`` utilizes OpenMP (or C++ ``std::thread``) 
     to distribute the workload of generating massive asset price 
     paths across multiple CPU cores.

**Workflow:**
The user executes a Python script detailing the option and simulation 
parameters. 

The Python frontend passes these via pybind11 to the C++ backend. The 
C++ ``MonteCarloPricer`` spawns multiple threads, each safely 
generating its own price paths using the RNG module and calculating 
the discounted payoff based on the ``Option`` module. 

Finally, the threads reduce their results to compute the average 
price, which is returned through pybind11 to Python for output.

API Description
===============

The system is designed to be programmed entirely in Python by the 
end-user, while the computationally intensive tasks are executed 
in C++. 

Users will import the compiled C++ extension as a standard Python 
module. Here is an example of how a user will write a script to 
price an option using the parallelized Monte Carlo engine:

.. code-block:: python

    import fastmcpricer as fmp

    # 1. Define the option contract
    # e.g., an Asian option with strike 100, maturity 1 year
    option = fmp.AsianOption(strike=100.0, maturity=1.0)

    # 2. Define the market data
    market = fmp.MarketData(spot=100.0, rate=0.05, volatility=0.2)

    # 3. Configure the engine and execute
    # Using 8 million paths and distributing the workload across 
    # 4 CPU threads
    result = fmp.price_option(
        engine="MonteCarlo",
        option=option,
        market=market,
        num_paths=8_000_000,
        num_threads=4
    )

    print(f"Calculated Price: {result.price}")
    print(f"Computation Time: {result.time_ms} ms")

Engineering Infrastructure
==========================

1. **Build System:** ``CMake`` will be used to manage the build 
   process. A single command will compile the C++ source code, link 
   the OpenMP (or internal threading) libraries, and build the 
   ``pybind11`` Python extension.
2. **Version Control:** Git and GitHub will be used. Feature 
   branches will be utilized for developing different pricing 
   engines and merging them into the main branch via Pull Requests.
3. **Testing Framework:** * **C++ Core:** ``Google Test (gtest)`` will 
     be used to verify the correctness of the mathematical models, 
     random number generation, and base option classes.
   * **Python Interface:** ``pytest`` will be used to validate the 
     end-to-end results. The Black-Scholes analytical solution will 
     serve as the baseline ground truth to test the convergence 
     accuracy of the Monte Carlo simulation.
4. **Continuous Integration (CI):** GitHub Actions will be 
   configured to automatically trigger the build and run all tests 
   upon every push to ensure code stability.
5. **Documentation:** The project will use ``Sphinx`` to generate 
   documentation from docstrings, detailing the API usage and 
   mathematical background.

Schedule
========

The project development is planned over an 8-week timeline 
(from 03/16 to 05/10):

* **Week 1 (03/23): Environment & Baseline:** Set up the CMake build 
  system, pybind11 integration, and implement the Black-Scholes 
  analytical solution as a testing baseline.
* **Week 2 (03/30): Core Math & Single-thread MC:** Implement the 
  Geometric Brownian Motion (GBM) logic and a single-threaded 
  Monte Carlo pricer for standard European options. Validate 
  correctness against the baseline.
* **Week 3 (04/06): RNG & Thread Safety:** Implement robust, 
  thread-safe random number generators (e.g., independent 
  ``std::mt19937`` states) in C++ to prepare for parallelization.
* **Week 4 (04/13): Parallelization:** Integrate OpenMP/``std::thread`` 
  to parallelize the path generation and payoff calculation. 
  Conduct preliminary speedup benchmarking.
* **Week 5 (04/20): Exotic Options:** Extend the base option 
  classes to include path-dependent derivatives, such as Asian and 
  Barrier options, utilizing the parallelized engine.
* **Week 6 (04/27): Python Integration & Visualization:** Refine 
  the Python API and write scripts using ``matplotlib`` to 
  visualize pricing convergence and multi-threading performance 
  scaling.
* **Week 7 (05/04): Profiling & Optimization:** Profile the C++ 
  code for memory leaks or race conditions. Optimize the reduction 
  process in the parallel loops.
* **Week 8 (05/11): Documentation & Wrap-up:** Finalize the Sphinx
  documentation, clean up the codebase, and prepare the 
  presentation slides.

References
==========

1. Hull, J. C. (2017). *Options, Futures, and Other Derivatives*.
2. Joshi, M. S. (2004). *C++ Design Patterns and Derivatives 
   Pricing*. Cambridge University Press.
