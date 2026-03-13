======================================
Stream Statistics for Real-time Data
======================================

**StreamStats**:
A high-performance C++11 library with Python bindings for real-time streaming
numerical statistics.

Basic Information
=================

GitHub Repository:

Numerical analysis often involves continuous data streams.
For example, high-frequency financial ticks, sensor telemetry, and other real-time signals.
Unlike static datasets, streaming data must be processed immediately.
Data points arrive sequentially and can be too voluminous to store entirely in memory.

Efficiently computing statistics over a "sliding window" (the most recent
:math:`W` elements) is a key operation in these domains. This project, StreamStats,
offers a C++ infrastructure to handle these tasks with minimal overhead,
while providing a seamless interface for Python-based data science workflows.

Problem to Solve
================

The primary challenge in streaming statistics is the trade-off between **computational latency** and **numerical stability** in high-frequency environments.

1. **Redundant Computation:** A naive implementation recomputes statistics from scratch each time the window advances. For a stream of length :math:`N` and a window size :math:`W`, this requires :math:`O(NW)` operations. As :math:`W` grows, latency becomes a bottleneck in real-time systems. StreamStats uses incremental algorithms to reduce total complexity to :math:`O(N)`.

2. **Memory Overhead and Jitter:** In Python-based pipelines, array slicing (e.g., ``data[-W:]``) can create memory copies. Frequent allocations may trigger garbage collection, causing unpredictable timing jitter. By using a C++ ``RingBuffer``, StreamStats keeps memory usage fixed and avoids expensive copying.

3. **Numerical Instability:** Naive variance formulas are prone to catastrophic cancellation due to floating-point precision limits. StreamStats uses Welford’s algorithm to maintain robust and accurate results, even in long-running streams.

To address these issues, StreamStats implements incremental C++ algorithms that update statistics in :math:`O(1)` or :math:`O(log W)` time, while efficiently sharing memory buffers with Python to minimize unnecessary copying.

Prospective Users
=================

StreamStats is intended for students, researchers, and developers who need to compute
basic streaming statistics with low latency while still working in Python.

Potential users include:

- **Quantitative Finance Learners and Practitioners:** Users working with market time-series
  data who need rolling statistics (e.g., moving average or volatility) updated as new ticks arrive.

- **IoT and Monitoring Developers:** Users building sensor-data pipelines that require
  real-time statistical summaries for simple anomaly detection.

- **Signal Processing and Experimental Research Students:** Users handling continuous
  measurement data (e.g., lab instruments) and applying window-based statistics for preprocessing.

Overall, StreamStats provides a C++ implementation for performance-critical updates and
a Python interface for easier experimentation and integration into data workflows.

System Architecture
===================

StreamStats follows a two-layer architecture to balance performance and usability:
a C++ core for streaming computation and a Python interface for user-facing workflows.

1. C++ Core Layer
-----------------
The core layer is implemented in C++11 and focuses on fixed-cost updates for
streaming statistics.

- **RingBuffer<T>** is the primary data container. It stores the most recent
  :math:`W` samples in a fixed-size contiguous buffer, so memory usage remains bounded.
- **Statistics modules** are updated incrementally when a new value arrives.
  The first target is rolling mean and variance (Welford-based update).
  Additional statistics (e.g., median) can be added as separate modules later.
- This layer is designed to minimize repeated computation and avoid frequent
  memory allocation during streaming.

2. Python Binding Layer
-----------------------
The Python layer is built with pybind11 and provides Python API bindings
for data analysis workflows.

- Python users can push new samples and query current statistics directly.
- The initial implementation will expose concrete floating-point types
  (for example, ``float`` and ``double`` bindings).

3. Data Flow and Scope
----------------------
At each update step, a new sample is inserted into the ring buffer, expired data
is logically removed, and enabled statistics are updated incrementally.

The first version focuses on correctness, numerical stability, and predictable
runtime behavior in a single-thread setting. Thread-safe updates and more advanced
memory-sharing strategies are considered future extensions.

API Description
===============

StreamStats provides a minimal API in both C++ and Python for rolling
statistics on streaming scalar data.  The first version focuses on essential
operations and keeps advanced features as future work.

In C++, a class template ``ScalarStream<T>`` will be provided under namespace
``streamstats``.  The constructor takes the window size, and the object supports
incremental updates and basic statistic queries.

Planned core methods in C++:

- ``ScalarStream<T>(std::size_t window_size)``
- ``void push(T value)``
- ``std::size_t size() const``
- ``T mean() const``
- ``T variance() const``
- ``void reset()``

In Python, ``pybind11`` bindings will expose concrete floating-point types (for
example, ``StreamStatsF64`` and ``StreamStatsF32``) with similar method names,
so users can run streaming updates in Python while using the C++ core.

The first version supports single-value updates through ``push()`` only.
Batch input support is considered optional future work.

Schedule
========

Week 1 (03/16 to 03/22):

 Project setup (GitHub repo, Makefile, pybind11 minimal build).

Week 2 (03/23 to 03/29):

 Implement ``RingBuffer<T>`` with fixed-size storage and wrap-around logic.

Week 3 (03/30 to 04/05):

 Implement rolling mean/variance (Welford-based update).

Week 4 (04/20 to 04/26): (after midterm exam week)

 Add basic C++ tests and fix core edge cases.

Week 5 (04/27 to 05/03):

 Implement Python bindings for ``float``/``double`` versions.

Week 6 (05/04 to 05/10):

 Add pytest tests and compare selected outputs with NumPy.

Week 7 (05/11 to 05/17):

 Perform simple performance checks against a naive Python baseline.

Week 8 (05/18 to 05/24):

 Buffer week for bug fixes, documentation cleanup, and final presentation preparation.

Optional Extensions
====================

If the core milestones are completed on time, the following topics may be explored:

1. Rolling median support.
2. Batch update API (e.g., ``push_batch()``).
3. Better NumPy interoperability (e.g., window export).
4. Additional benchmark cases.
5. Preliminary thread-safety design discussion.

These items are optional and not required for the first project milestone.

