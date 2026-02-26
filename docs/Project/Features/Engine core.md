---
tags: [feature]
epic: "[[Epics/v0.1]]"
created: 2026-02-25
updated: 2026-02-25
status: complete
---

# Feature: Engine Core

## Overview

The heart of the Uinta Engine - manages the main game loop, scene queueing, event dispatching, and frame timing across multiple engine stages.

## Requirements

- Main engine loop with configurable stages
- Scene queue management
- Event dispatcher system
- Frame timing and delta time tracking
- Engine state management
- Multi-stage pipeline support

## Implementation Details

### Engine Stages

The engine operates in six distinct stages per frame:

1. **PreTick** - Pre-simulation setup
2. **Tick** - Main simulation/game logic
3. **PostTick** - Post-simulation cleanup
4. **PreRender** - Pre-render setup
5. **Render** - Main rendering
6. **PostRender** - Post-render cleanup

### Engine State

**EngineState** manages:

- Runtime tracking (total elapsed time)
- Per-stage delta time
- Frame synchronization

### Event System

**EngineEvents** supports:

- `RenderComplete` - Fired after render stage
- `TickComplete` - Fired after tick stage
- `ViewportSizeChange` - Window/viewport resize events
- `RenderLayerChange` - Render layer switching

## Usage Example

```cpp
uinta::Engine engine({&platform, &appConfig});
engine.addScene<DemoScene>();
engine.run();
```
