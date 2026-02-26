---
tags: [feature]
epic: "[[Epics/v0.1]]"
created: 2026-02-25
updated: 2026-02-25
status: complete
---

# Feature: System Architecture

## Overview

System-based scene logic for scene-wide operations and management, complementing the component system.

## Requirements

- [x] Base System class for scene-wide logic
- [x] SystemManager for lifecycle and updates
- [x] Stage-based virtual methods
- [x] Integration with scene lifecycle

## Implementation Details

### System Base Class

**System:**
Virtual methods for each engine stage:

- `onPreTick(state)` - Pre-simulation logic
- `onTick(state)` - Main system logic
- `onPostTick(state)` - Post-simulation logic
- `onPreRender(state)` - Pre-render setup
- `onRender(state)` - Rendering operations
- `onPostRender(state)` - Post-render cleanup

### SystemManager

**Responsibilities:**

- System lifecycle management
- Update dispatch across engine stages
- System registration and initialization
- Memory management

## Usage Example

```cpp
class PhysicsSystem : public uinta::System {
  void onTick(const uinta::EngineState& state) override {
    // Update all physics entities
  }
};

scene.addSystem<PhysicsSystem>();
```
