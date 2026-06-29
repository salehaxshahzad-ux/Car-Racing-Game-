# Car-Racing-Game-
An object-oriented C++ game featuring abstract classes, composition, and non-blocking multi-threaded key inputs via _kbhit(). Supports dual-player coordination over a 5-lane matrix grid.

# OOP Concepts Implemented:

# Abstraction:
Hides the complex driving mechanics and keyboard buffer reading behind a clean, unified public interface.

# Inheritance:
Establishes a reusable parent-child hierarchy by deriving playerCar and enemyCar from a shared abstract vehicle base class.

# Polymorphism:
Enables runtime dynamic binding by overriding the pure virtual move() function to trigger distinct behaviors for players and enemies.

# Encapsulation:
Protects data integrity by restricting direct access to vehicle lanes and color variables, exposing them only through secure public getter methods.

# Composition:
Demonstrates a "Has-A" architecture by instantiating and controlling multiple player and enemy objects inside the central game engine class.

# Key Highlights:

Dual Inputs: Clear breakdown of Player 1 ('A'/'D') and Player 2 (Arrows '</>').

The 5-Lane Highway: Highlighting the expanded road track layout.

Dynamic Visuals: Mention of randomized ASCII characters and custom Windows API terminal colors.


