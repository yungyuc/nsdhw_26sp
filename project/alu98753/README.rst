========================================================================
LockFreeReplay: A Zero-Copy Lock-Free Ring Buffer for RL
========================================================================

`[Chinese Version] <README.zh-tw.rst>`__

Basic Information
=================

LockFreeReplay is a C++ library providing a single-producer/single-consumer (SPSC) lock-free ring buffer for experience replay in reinforcement learning (RL), coupled with a zero-copy Python interface. 

The code will be hosted in a new repository: https://github.com/alu98753/lockfree-replay

In distributed or high-throughput RL data pipelines, memory buffers are required to store transition data (e.g., state, action, reward). There are two major bottlenecks in existing implementations:

1. Storing data in Python or using lock-protected C++ queues leads to Global Interpreter Lock (GIL) contention and thread stalling during concurrent read/write operations.
2. Transferring data between C++ (environment runners/replay buffers) and Python (training loops) involves memory copying, which consumes CPU cycles and memory bandwidth.

Problem to Solve
================

The primary problem LockFreeReplay tries to solve is to eliminate lock contention on the replay data path and share memory buffers between C++ and Python (NumPy) without copying.

Multi-dimensional arrays holding RL transitions require fast concurrent writes from the actor (producer) and fast batch sampling from the learner (consumer). Standard Python-based multiprocessing or lock-based C++ queues suffer from locking overhead. Passing sampled mini-batches back to Python typically results in memory copies.

LockFreeReplay focuses on providing a lock-free C++ data structure to solve the writing contention and exposing the underlying contiguous memory directly to Python via pybind11's buffer protocol to avoid copying.

Prospective Users
=================

Researchers and engineers building single-node RL pipelines who need high-throughput data collection without the overhead of heavy distributed systems (like Reverb) or Python multiprocessing. 

System Architecture
===================

LockFreeReplay will be developed in C++ using atomic operations for head and tail indices to achieve a lock-free SPSC ring buffer. It manages untyped memory buffers for fixed-layout transitions.

In C++, a class ``ReplayBuffer`` allocates a contiguous memory block. It does not use mutexes for concurrent read/write operations from a single producer and a single consumer thread.

In Python, the C++ buffer is wrapped using pybind11. It exposes views of the memory block directly as NumPy ``ndarray`` via ``pybind11::buffer_info``. 

Users are enabled to push transition data from Python (or C++) and sample mini-batches in Python. The sampling function returns NumPy views pointing exactly to the C++ memory block, ensuring zero-copy.

Note
----

* By lock-free, the implementation uses ``std::atomic`` and memory ordering, specifically avoiding ``std::mutex``.
* The scope is constrained to a single-producer, single-consumer (SPSC) bounded queue with an overwrite-oldest policy. Multi-producer scenarios are not supported in this initial version.
* The Python view lifetime must not outlive the C++ buffer allocation; this will be managed via smart pointers (e.g., ``std::shared_ptr``).

API Description
===============

The main class ``ReplayBuffer`` will have constructors taking buffer capacity and transition layout (e.g., shapes of state and action).

C++ methods will include ``push()`` for appending data and ``sample()`` for retrieving data indices. 

The Python interface, bound via pybind11, will provide:
* ``buffer.push_batch(ndarray)``: Accepts NumPy arrays containing transitions.
* ``buffer.sample_views(batch_size)``: Returns a dictionary of NumPy arrays representing the sampled mini-batch. These arrays are zero-copy views of the C++ memory.

Engineering Infrastructure
==========================

* **Build System**: CMake configures the project and builds the pybind11 module.
* **Testing**: Google Test (C++) tests the correctness of the lock-free logic (e.g., no data loss during concurrent push/sample). ThreadSanitizer (TSan) will be utilized to detect data races. ``pytest`` (Python) asserts zero-copy behavior (memory address checking) and functional correctness.
* **Version Control**: Git tracks changes, with regular commits demonstrating progress.
* **Continuous Integration**: GitHub Actions automatically runs the C++ and Python test suites on Linux for each commit.
* **Performance Measurement**: Throughput (operations per second) and latency percentiles will be tracked using standard C++ timing libraries to prove the effectiveness of the lock-free design against a mutex-based baseline.

Schedule
========

Week 1 (03/16 - 03/22):
  Define the transition layout structure. Implement a baseline mutex-based ring buffer in C++ and write single-threaded correctness tests using Google Test.

Week 2 (03/23 - 03/29):
  Refactor the C++ ring buffer to an SPSC lock-free implementation using ``std::atomic``. Implement cache line alignment (``alignas(64)``) to prevent false sharing. 

Week 3 (03/30 - 04/05):
  Write multi-threaded stress tests (1 producer, 1 consumer). Configure ThreadSanitizer (TSan) via CMake and verify the absence of data races.

Week 4 (04/06 - 04/12):
  Implement the pybind11 wrapper. Expose the buffer creation and basic push methods to Python.

Week 5 (04/13 - 04/19):
  Develop the zero-copy view mechanism. Implement ``sample_views()`` to return NumPy arrays using ``pybind11::buffer_info``. 

Week 6 (04/20 - 04/26):
  Manage object lifetime via ``std::shared_ptr`` to ensure Python views remain safe. Write Python integration tests (``pytest``) asserting memory sharing.

Week 7 (04/27 - 05/03):
  Set up GitHub Actions for CI. Write benchmark scripts to compare the lock-free implementation's throughput vs. the mutex baseline.

Week 8 (05/04 - 05/10):
  Finalize documentation, clean up code, and prepare the final presentation.

References
==========

* cpprb (C++ Replay Buffer for RL): https://github.com/ymd-h/cpprb
* modmesh (SimpleArray reference): https://github.com/solvcon/modmesh
