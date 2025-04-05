# Polygine

A minimalist, code-first game engine written in C++.

## Overview

Polygine (Poly + Engine) is a lightweight, modular game engine designed with simplicity at its core. Taking inspiration from SFML's straightforward API and Raylib's focus on making game development fun, Polygine aims to provide a clean, intuitive interface for game creators.

## Philosophy

- **Code-First Approach**: Build your games through code with minimal abstraction layers
- **Simplicity**: Easy to learn, easy to use
- **Modularity**: Use only what you need
- **Cross-Platform**: Plans to support major desktop platforms, mobile, and console

## Technical Architecture

- **ECS (Entity Component System)**: Data-oriented programming for efficient game scene management
- **Event System**: Flexible communication between game systems
- **Hybrid Architecture**: Uses the best approach for each subsystem (ECS for game logic, OOP for complex pipelines)
- **React-like UI System**: Modern, declarative UI built on the engine's renderer

## Current Status

Polygine is in very early development stages. Currently implementing:

- Core ECS architecture
- Event system
- Foundation for future systems

Coming soon:
- Window management
- Input handling
- Graphics rendering
- Physics
- Audio

## Future Plans

- **Game Engine Editor**: Customizable editor built using the engine itself
- **Extensibility**: Support for plugins and custom tools
- **Full Open-Source Release**: When the project reaches maturity

## Disclaimer

This is currently a hobby project with the goal of becoming a fully-fledged open-source engine in the future.

## License

This project is licensed under the MIT License - see the [LICENSE.md](LICENSE.md) file for details.
