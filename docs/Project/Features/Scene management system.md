---
tags: [feature]
epic: "[[Epics/v0.1]]"
created: 2026-02-25
updated: 2026-02-25
status: complete
---

# Feature: Scene Management System

## Overview

Hierarchical scene management system supporting scene stacking, layering, and state transitions with component composition.

## Requirements

- [x] Base Scene class with lifecycle hooks
- [x] Scene stacking with layer support
- [x] Scene state management (Running/Paused/Complete)
- [x] Scene composition with components and systems
- [x] Scene event dispatching
- [x] Hierarchical scene tree support

## Implementation Details

### Scene Framework

**Scene Base Class:**

- Virtual lifecycle methods: `onPreTick`, `onTick`, `onPostTick`, `onPreRender`, `onRender`, `onPostRender`
- Component and system attachment
- Hierarchical parent-child relationships
- Debug UI hooks (`onDebugUi`)

### Scene Layers

**SceneLayer Enum:**

- `Simulation` - Main game/simulation layer
- `Debug` - Debug visualization layer

Scenes can be organized by layer for proper rendering order and separation of concerns.

### Scene States

**SceneState:**

- `Running` - Scene is actively updating and rendering
- `Paused` - Scene is paused (no updates, still renders)
- `Complete` - Scene has finished execution

### Scene Stack

**SceneStack:**

- Manages ordered collection of scenes
- Layer-based organization
- Scene activation/deactivation
- Scene ordering and prioritization

### Scene Events

**SceneEvents:**

- `SceneAdded` - New scene added to stack
- `SceneLayerChange` - Scene layer changed
- `SceneStateChange` - Scene state transition

## Usage Example

```cpp
class MyGameScene : public uinta::Scene {
  void onTick(const uinta::EngineState& state) override {
    // Game logic here
  }

  void onRender(const uinta::EngineState& state) override {
    // Rendering here
  }
};

engine.addScene<MyGameScene>();
```
