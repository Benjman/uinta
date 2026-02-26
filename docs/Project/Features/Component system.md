---
tags: [feature]
epic: "[[Epics/v0.1]]"
created: 2026-02-25
updated: 2026-02-25
status: complete
---

# Feature: Component System

## Overview

Component-based architecture enabling modular entity composition with lifecycle-aware updates across engine stages.

## Requirements

- [x] Base Component class with lifecycle types
- [x] Stage-specific component types (PreTick, Tick, PostTick, PreRender, Render, PostRender)
- [x] Interval-based components with scheduling
- [x] Priority queue for interval components

## Implementation Details

### Component Types

**Lifecycle Components:**

- `PreTickComponent` - Updates before main tick
- `TickComponent` - Main simulation updates
- `PostTickComponent` - Updates after main tick
- `PreRenderComponent` - Pre-render setup
- `RenderComponent` - Rendering operations
- `PostRenderComponent` - Post-render cleanup

**Special Components:**

- `IntervalComponent` - Time-based updates with configurable intervals
- Priority queue scheduling for interval components

### Component Manager

- Automatic component registration and deregistration
- Stage-based update dispatch
- Memory management for component instances

## Usage Example

```cpp
class MyComponent : public uinta::TickComponent {
  void tick(const uinta::EngineState& state) override {
    // Component logic
  }
};

scene.addComponent<MyComponent>(args...);
```
