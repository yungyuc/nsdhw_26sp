=================
    DotArena
=================

Basic Information
=================

**Repository:** https://github.com/romanycc/DotArena

DotArena is a lightweight 3D physics simulation engine built in C++ with Python
bindings, focusing on elastic collisions, friction, and dynamic boundary
mechanics for battle-royale-style environments.

Problem to Solve
================

This project aims to build a high-performance 3D particle simulation engine
capable of handling massive scales (:math:`N = 10^5` to :math:`10^6` spheres).
Moving from 2D to 3D makes the simulation much more complex. At this scale,
checking every single pair of particles for collisions is simply too slow for
any computer to handle.

1. **Newton's Laws of Motion & Friction**:

   * *First Law (Inertia)*: The dots maintain their velocity and direction until
     acted upon by an external force (friction or collision).
   * *Second Law (F=ma)*: The 3D space applies a constant friction force,
     causing the moving dots to decelerate over time.
   * *Third Law (Action and Reaction)*: When two dots collide, they exert equal
     and opposite forces on each other.

2. **The Collision Mechanism**: Implementing 3D elastic collisions. The system
   must accurately calculate the exchange of kinetic energy and the new
   directional vectors for both dots based on the conservation of momentum
   (:math:`m_1v_1 + m_2v_2 = m_1v_1' + m_2v_2'`), taking their respective masses
   and velocities into account.

3. **Boundary Constraints**: The 3D space constantly shrinks over time. If a
   dot's coordinates fall outside the current boundary radius, it is eliminated
   ("dead").

4. **3D Spatial Partitioning**: Implementing and benchmarking advanced 3D
   spatial data structures (e.g., 3D Uniform Grid vs. Octree vs. KD-Tree)
   for broad-phase collision detection to reduce time complexity.

5. **Memory Architecture & Cache Locality**: Shifting from traditional Object-
   Oriented Programming (AoS) to Data-Oriented Design (SoA) to maximize CPU
   cache hit rates and SIMD vectorization during intensive 3D state updates.

6. **Parallelism & Profiling**: Utilizing OpenMP for multi-threading. Rigorous
   performance profiling (using tools like Nsight or perf) will be conducted
   to identify and resolve bottlenecks in 3D vector mathematics.

7. **Memory Sharing**: Efficiently sharing massive 3D coordinate buffers with
   Python via ``pybind11`` without copying overhead, allowing for advanced 3D
   visualization using tools.

Prospective Users
=================

The big picture of this motion simulation is to serve as a computational
sandbox. It observes emergent behaviors from basic kinematics and collision
mechanics. The prospective users include:

1. **Game Developers**: Those who need a fast, reliable C++ backend to handle
   collision detection and boundary logic for top-down games.
2. **Physics Students & Educators**: Users who want to visualize momentum,
   Newton's laws, and energy transfer in an interactive script.
3. **AI/ML Researchers**: Users looking for a lightweight, customizable 3D
   environment to train reinforcement learning agents.

System Architecture
===================

To simplify the problem, the system transforms the scenario into multiple dots
with specific masses and radii on a 3D space.

**Workflow**:

1. **Initialization**: The user defines the initial states (position, mass,
   size, velocity) of :math:`N` dots and the plane's parameters.
2. **Time-stepping (The Loop)**: At each timestamp (:math:`\Delta t`), the
   system updates the simulation:

   * **Kinematics**: Updates each dot's position based on its current velocity.
   * **Friction**: Reduces the velocity of each moving dot based on the plane's
     friction coefficient.
   * **Collision Detection & Resolution**: Checks for overlaps and assigns new
     velocity vectors.
   * **Boundary Update & Elimination**: Reduces the plane's boundary size and
     marks dots outside the boundary as dead.

3. **Memory Sharing**: The C++ backend handles the intensive collision math
   while exposing memory buffers to Python (NumPy) for easy visualization
   without massive copying overhead.

API Description
===============

The system will be implemented in C++ for performance, with a Python wrapper.

* ``class Dot``: Holds properties including ``id``, ``radius``, ``mass``,
  ``position``, ``velocity``, and ``is_alive``.
* ``class Plane``: Manages ``current_radius``, ``friction_coeff``, and
  ``shrink_rate``.
* ``class Simulation``: The orchestrator. It advances the simulation by one
  timestamp via the ``step(dt)`` method.

.. code-block:: python

    import dotarena

    # Initialize plane and simulation
    arena = dotarena.Plane(initial_radius=1000.0, friction_coeff=0.05,
                           shrink_rate=10.0)
    sim = dotarena.Simulation(plane=arena)

    # Create and add dots
    dot1 = dotarena.Sphere(id=1, mass=10.0, radius=5.0, pos=(0.0, 0.0, 0.0),
                           vel=(50.0, 20.0, -10.0))
    sim.add_dot(dot1)

    # Advance simulation and extract state
    sim.step(dt=0.016)
    active_dots = sim.get_active_dots()

Engineering Infrastructure
==========================

1. **Automatic Build System**: ``CMake`` and ``pybind11`` will be used for
   C++ compilation and Python wrapping.
2. **Version Control**: Git/GitHub using a standard branch-and-merge workflow.
3. **Testing Framework**:

   * Python level: ``pytest`` for API bindings and integration tests.

Schedule
========

* **Week 1 (03/07 to 03/13)**: Set up the GitHub repository, CMake build system,
  and basic C++ class definitions (``Dot``, ``Plane``, ``Simulation``).
* **Week 2 (03/14 to 03/20)**: Implement the core kinematics math in C++:
  Newton's laws, position updates, and the friction deceleration logic.
* **Week 3 (03/21 to 03/27)**: Implement the 3D collision detection and
  resolution engine (momentum conservation), as well as the boundary shrinking
  logic.
* **Week 4 (03/28 to 04/03)**: Python Binding & Integration Testing. Set up the
  Python interface for the physics engine using ``pybind11``.
* **Week 5 (04/04 to 04/10)**: Write Python-driven test scripts (e.g., using
  ``pytest`` and ``numpy``) to verify C++ physics accuracy, ensure correct
  cross-language data conversion, and handle edge cases (e.g., multiple
  simultaneous collisions) directly from the Python environment.
* **Week 6 (04/11 to 04/17)**: Write Python testing scripts (``pytest``) and
  develop a simple visualizer (e.g., using ``matplotlib`` or ``pygame``) to
  render the simulation.
* **Week 7 (04/18 to 04/24)**: Refactoring, performance profiling, and bug
  fixing. Ensure no memory leaks exist between the C++ and Python boundary.
* **Week 8 (04/25 to 05/01)**: Finalize documentation, clean up the codebase,
  and prepare the final presentation/demo.

References
==========

* `Roblox Knockout Mechanics <https://youtube.com/shorts/rhaI_gnXczo>`_