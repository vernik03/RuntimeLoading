# Runtime Loading in Unreal Engine

This project implements runtime asset loading and manipulation in Unreal Engine 5.5 with collision handling and mesh processing capabilities.

## Features

- **JSON Parsing**: Parse JSON files to retrieve links for USD environment assets and glTF meshes
- **Mesh Processing**:
  - Load meshes at runtime
  - Generate collision data
  - Combine multiple static meshes into procedural meshes for collision detection during movement
- **Mesh Manipulation**:
  - Choose meshes from a menu and place them in the scene
  - Transform controls (rotation, scaling, movement)
  - Collision checking during movement (toggleable)
- **Scene Editing**:
  - Select existing placed meshes to modify their transforms
  - Update position, rotation, and scale of scene objects

## Demonstration

[![Video Demonstration](https://img.youtube.com/vi/PtwMOHVv9Rc/0.jpg)](https://www.youtube.com/watch?v=PtwMOHVv9Rc)
