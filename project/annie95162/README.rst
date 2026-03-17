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

1. **C++ Core Library**: Manages contiguous memory allocation (using a custom 
   array structure similar to a simplified ``std::vector`` or 2D buffer) 
   and performs heavy numerical distance calculations.
2. **Python Interface**: Built with ``pybind11`` to expose C++ structures 
   to Python, allowing direct interaction with ``numpy.ndarray``.

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
* **Testing Framework**: ``pytest`` for the Python API and ``GTest`` or 
  simple C++ assertions for the core.
* **Documentation**: ``reStructuredText`` included in the repository.

Schedule
========

Planning phase (8 weeks):

* **Week 1 (03/16)**: Finalize C++ data structure design and setup CMake with pybind11.
* **Week 2 (03/23)**: Implement core ``VectorStore`` class and memory management.
* **Week 3 (03/30)**: Implement optimized L2 distance computation kernels.
* **Week 4 (04/06)**: Implement Cosine similarity and edge-case handling.
* **Week 5 (04/13)**: Develop the k-Nearest Neighbor search algorithm (Brute-force/Flat).
* **Week 6 (04/20)**: Finalize Python bindings and ensure NumPy compatibility.
* **Week 7 (04/27)**: Performance profiling and optimization (OpenMP).
* **Week 8 (05/04)**: Complete documentation and prepare for presentation.

References
==========

1. Johnson, J., Douze, M., & Jégou, H. (2019). "Faiss: A library for efficient similarity search."
2. pybind11 documentation: https://pybind11.readthedocs.io/
3. Rebuffi, S. A., et al. (2017). "iCaRL: Incremental Classifier and Representation Learning."
