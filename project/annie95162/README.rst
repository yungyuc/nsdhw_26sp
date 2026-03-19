=========================================
ProtoLib: Incremental Vector Store
=========================================

Basic Information
=================

Project Repository:
https://github.com/annie95162/ProtoLib

ProtoLib is a lightweight vector storage and similarity search library
designed for incremental learning systems. The library supports efficient
storage and retrieval of feature vectors and provides high-performance
nearest neighbor search functionality.

Problem to Solve
================

In many machine learning systems, especially incremental learning and
continual learning, models need to store representative feature vectors
(prototypes) of previously learned classes.

These stored vectors are later used for tasks such as:

* Nearest neighbor classification
* Prototype-based inference
* Rehearsal-free continual learning
* Similarity search

Typical solutions like FAISS or Milvus are powerful but often introduce
excessive overhead and complexity for moderate-scale research tasks or
embedded environments.

The goal of this project is to implement an efficient vector storage system
optimized for numerical performance:

* **Dynamic insertion**: Efficiently manage memory for growing datasets.
* **Numerical optimization**: Implement L2 and Cosine similarity using C++
  loops with potential SIMD (AVX/NEON) or OpenMP acceleration.
* **Zero-copy interface**: Provide a seamless Python binding to handle NumPy
  arrays without unnecessary memory duplication.

Prospective Users
=================

* Machine learning researchers focusing on incremental learning.
* Developers building lightweight prototype-based classifiers for edge devices.
* Students who need a transparent and hackable vector store for educational purposes.

System Architecture
===================

The system follows a classic hybrid architecture:

1. **C++ Core Library**: Manages contiguous memory using a custom buffer
   abstraction instead of directly relying on ``std::vector``, ensuring
   stable memory layout and safe interaction with Python.

2. **Python Interface**: Built with ``pybind11`` to expose C++ structures.
   The interface uses the buffer protocol to support efficient data exchange
   with NumPy arrays while avoiding memory safety issues.

**Work Flow**:
Input vectors (NumPy) -> pybind11 (Pointer passing) -> C++ Vector Store -> 
SIMD-optimized Search -> Results returned to Python.

**Modularization**:

* ``BufferManager``: Handles memory allocation and resizing.
* ``DistanceEngine``: Contains the numerical kernels (L2/Cosine).
* ``PythonWrapper``: The ``pybind11`` binding layer.
* ``TestSuite``: Unit tests for both C++ and Python.

API Description
===============

The system provides both a C++ API and a Python API.

The C++ API
-----------

The core functionality is implemented in C++. The main class
``VectorStore`` manages vector storage and similarity search.

Vectors are stored in a contiguous memory layout using a single
``std::vector<float>`` buffer.

.. code-block:: cpp

    class VectorStore{
    public:

        // constructor
        VectorStore(size_t dimension);

        // add vectors to the store
        // vectors.size() must equal n_vectors * dimension
        void add(const std::vector<float>& vectors, size_t n_vectors);

        // compute k-nearest neighbors using L2 distance
        std::pair<std::vector<size_t>, std::vector<float>> search_l2(
            const std::vector<float>& query,
            size_t k
        ) const;

        // compute k-nearest neighbors using cosine similarity
        std::pair<std::vector<size_t>, std::vector<float>> search_cosine(
            const std::vector<float>& query,
            size_t k
        ) const;

        // return number of stored vectors
        size_t size() const;

    private:

        size_t m_dim;

        // contiguous storage: size = num_vectors * m_dim
        std::vector<float> m_storage;
    };

Main API functionalities include:

* ``VectorStore(dim)`` — initialize a vector store with fixed dimension
* ``add(vectors, n_vectors)`` — append new vectors to the storage buffer
* ``search_l2(query, k)`` — return k nearest neighbors using Euclidean distance
* ``search_cosine(query, k)`` — return k nearest neighbors using cosine similarity
* ``size()`` — return number of stored vectors

The current implementation uses brute-force search over all vectors.
Future work may include SIMD optimization or approximate nearest
neighbor methods.

The Python API
--------------

The C++ implementation will be exposed to Python using ``pybind11``.
The Python interface supports NumPy arrays through the buffer protocol.

Example usage:

.. code-block:: python

    import protolib
    import numpy as np

    store = protolib.VectorStore(dim=128)

    vectors = np.random.rand(100, 128).astype('float32')
    store.add(vectors)

    query = np.random.rand(128).astype('float32')

    ids, distances = store.search_l2(query, k=5)

The Python API supports the following operations:

* ``VectorStore(dim)`` — initialize a vector store
* ``add(vectors)`` — insert NumPy vectors into the store
* ``search_l2(query, k)`` — perform L2 nearest neighbor search
* ``search_cosine(query, k)`` — perform cosine similarity search

Engineering Infrastructure
==========================

* **Build System**: CMake. Support building the entire project with a single 
  command (e.g., ``pip install .`` or ``cmake && make``).
* **Version Control**: Git on GitHub. Maintaining a clean commit history 
  following the "Conventional Commits" style.
* **Testing Framework**: Testing will be divided into two levels:
* **C++ Testing**
  - verify memory correctness
  - validate distance computations
  - test edge cases (empty input, dimension mismatch)
* **Python Testing**
  - validate NumPy input/output behavior
  - ensure buffer safety and no memory corruption
  - integration tests for API usage
  Python tests will use ``pytest``.
  C++ tests will use simple assertions or a lightweight framework.
* **Documentation**: ``reStructuredText`` included in the repository.

Schedule
========

Planning phase (10 weeks):

* **Week 1 (03/16)**: Finalize system design and define vector storage data 
  structure.
* **Week 2 (03/23)**: Implement basic memory layout and contiguous buffer 
  management.
* **Week 3 (03/30)**: Add unit tests for storage correctness and validate memory 
  behavior.
* **Week 4 (04/06)**: Implement L2 distance computation and corresponding tests.
* **Week 5 (04/13)**: Implement cosine similarity and handle edge cases.
* **Week 6 (04/20)**: Develop the k-Nearest Neighbor (brute-force) search algorithm.
* **Week 7 (04/27)**: Integrate components and verify correctness of search results.
* **Week 8 (05/04)**: Implement Python bindings using pybind11 and test NumPy 
  compatibility.
* **Week 9 (05/11)**: Perform performance profiling and basic optimization 
  (e.g., OpenMP).
* **Week 10 (05/18)**: Finalize documentation and prepare presentation.

References
==========

1. Johnson, J., Douze, M., & Jégou, H. (2019). "Faiss: A library for
   efficient similarity search."
2. pybind11 documentation: https://pybind11.readthedocs.io/
3. Rebuffi, S. A., et al. (2017). "iCaRL: Incremental Classifier and
   Representation Learning."
