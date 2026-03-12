=================
DotArena
=================

Basic Information
=================

**Repository:** https://github.com/romanycc/DotArena

DotArena is a lightweight 2D physics simulation engine built in C++ with Python 
bindings, focusing on elastic collisions, friction, and dynamic boundary 
mechanics for battle-royale-style environments.

Problem to Solve
================

This project aims to simulate a 2D physical environment where multiple entities 
(represented as dots) interact under the constraints of classical mechanics and 
a dynamically shrinking boundary. The core physics problems solved by this 
engine include:

1. **Newton's Laws of Motion & Friction**: 

   * *First Law (Inertia)*: The dots maintain their velocity and direction until
    acted upon by an external force (friction or collision).
   * *Second Law (F=ma)*: The 2D plane applies a constant friction force, 
   causing the moving dots to decelerate over time.
   * *Third Law (Action and Reaction)*: When two dots collide, they exert equal
    and opposite forces on each other.

2. **The Collision Mechanism**: Implementing 2D elastic collisions. The system 
must accurately calculate the exchange of kinetic energy and the new directional
 vectors for both dots based on the conservation of momentum 
 (:math:`m_1v_1 + m_2v_2 = m_1v_1' + m_2v_2'`), taking their respective masses 
 and velocities into account.

3. **Boundary Constraints**: The 2D plane constantly shrinks over time. If a 
dot's coordinates fall outside the current boundary radius, it is eliminated 
("dead").

Prospective Users
=================

The big picture of this motion simulation is to serve as a computational sandbox
. It observes emergent behaviors from basic kinematics and collision mechanics. 
The prospective users include:

1. **Game Developers**: Those who need a fast, reliable C++ backend to handle 
collision detection and boundary logic for top-down or battle-royale-style games
 (similar to "knockout" mechanics).
2. **Physics Students & Educators**: Users who want to visualize momentum, 
Newton's laws, and energy transfer in an interactive, programmable script.
3. **AI/ML Researchers**: Users looking for a lightweight, customizable 2D 
environment to train reinforcement learning agents on survival or combat 
strategies within a shrinking arena.

System Architecture
===================

To simplify the problem, the system transforms the scenario into multiple dots 
with specific masses and radii on a 2D plane. 

**Workflow**:

1. **Initialization**: The user defines the initial states 
(position, mass, size, velocity) of :math:`N` dots and the plane's parameters 
(friction coefficient, initial size, shrinking rate).
2. **Time-stepping (The Loop)**: At each timestamp (:math:`\Delta t`), the 
system updates the simulation:

   * **Kinematics**: Updates each dot's position based on its current velocity.
   * **Friction**: Reduces the velocity of each moving dot based on the plane's 
   friction coefficient.
   * **Collision Detection & Resolution**: Checks for overlaps between any two 
   dots. If a collision occurs, it recalculates and assigns their new velocity 
   vectors.
   * **Boundary Update**: Reduces the plane's boundary size.
   * **Elimination**: Checks the coordinates of all dots. Any dot outside the 
   new boundary is marked as dead and removed from the active array.

3. **Memory Sharing**: The C++ backend will handle the intensive collision math,
 while exposing the memory buffers to Python (numpy) for easy visualization and
  state extraction without massive copying overhead.


API Description
===============

The system will be implemented in C++ for performance, with a Python wrapper for
 scriptability.

* ``class Dot``: Holds the properties of the entity, including ``id``, 
``radius`` (size), ``mass``, ``position (x, y)``, ``velocity (vx, vy)``, and 
``is_alive``. It provides accessors for the physics engine to update its state.
* ``class Plane``: Manages the environment variables. Contains properties like 
``current_radius``, ``friction_coeff``, and ``shrink_rate``. It includes methods
 like ``shrink_boundary()`` and ``is_out_of_bounds(Dot)``.
* ``class Simulation``: The orchestrator. It holds the array of ``Dot`` objects
 and the ``Plane``. It provides the ``step(dt)`` method to advance the 
 simulation by one timestamp, handling all collision resolutions and physics 
 updates internally.
.. code-block:: python

    import dotarena

    # Initialize a 2D plane (the arena)
    # input: initial radius, friction coefficient, and shrink rate per second
    arena = dotarena.Plane(initial_radius=1000.0, friction_coeff=0.05, shrink_rate=10.0)

    # Initialize the simulation orchestrator with the plane
    sim = dotarena.Simulation(plane=arena)

    # Create dots (entities)
    # input: id, mass, radius, position (x, y), velocity (vx, vy)
    dot1 = dotarena.Dot(id=1, mass=10.0, radius=5.0, pos=(0.0, 0.0), vel=(50.0, 20.0))
    dot2 = dotarena.Dot(id=2, mass=15.0, radius=7.5, pos=(100.0, 50.0), vel=(-30.0, 10.0))

    # Insert dots into the simulation
    sim.add_dot(dot1)
    sim.add_dot(dot2)

    # Advance the simulation by one timestamp (e.g., 0.016 seconds for ~60 FPS)
    # This handles kinematics, friction, collisions, and boundary shrinking internally
    # input: time step (dt)
    sim.step(dt=0.016)

    # Extract the state of all alive dots for visualization or analysis
    # return: a list/array of active dots with their current properties
    active_dots = sim.get_active_dots()

Engineering Infrastructure
==========================

1. **Automatic Build System**: ``CMake`` will be used to manage the C++ 
compilation and link the library. ``pybind11`` will be utilized to generate the
 Python wrappers.
2. **Version Control**: Git will be used for version control, hosted on GitHub.
 Features will be developed using a standard branch-and-merge workflow.
3. **Testing Framework**: 

   * C++ level: ``Google Test (gtest)`` to verify the accuracy of the collision
    math and momentum conservation.
   * Python level: ``pytest`` to test the API bindings and ensure script logic 
   behaves as expected.

4. **Documentation**: The documentation will be updated with each push to the 
GitHub repository.

Schedule
========

Development is planned for 8 weeks:

* **Week 1 (03/07 to 03/13)**: Set up the GitHub repository, CMake build system,
 and basic C++ class definitions (``Dot``, ``Plane``, ``Simulation``)
* **Week 2 (03/14 to 03/20)**: Implement the core kinematics math in C++: 
Newton's laws, position updates, and the friction deceleration logic.
* **Week 3 (03/21 to 03/27)**: Implement the 2D collision detection and 
resolution engine (momentum conservation), as well as the boundary shrinking 
logic.
* **Week 4 (03/28 to 04/03)**: Python Binding & Integration Testing. Set up the 
Python interface for the physics engine using ``pybind11``.
* **Week 5 (04/04 to 04/10)**: Write Python-driven test scripts 
(e.g., using ``pytest`` and ``numpy``) to verify C++ physics accuracy, ensure 
correct cross-language data conversion, and handle edge cases (e.g., multiple 
simultaneous collisions) directly from the Python environment.
* **Week 6 (04/11 to 04/17)**: Write Python testing scripts (``pytest``) and 
develop a simple visualizer (e.g., using ``matplotlib`` or ``pygame``) to render
 the simulation.
* **Week 7 (04/18 to 04/24)**: Refactoring, performance profiling, and bug 
fixing. Ensure no memory leaks exist between the C++ and Python boundary.
* **Week 8 (04/25 to 05/01)**: Finalize documentation, clean up the codebase, 
and prepare the final presentation/demo.

References
==========

* `Roblox Knockout Mechanics (Concept Inspiration) 
<https://youtube.com/shorts/rhaI_gnXczo?si=r_cio35Sf5GZvpM7>`