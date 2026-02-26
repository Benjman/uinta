---
tags: [feature]
epic: "[[Epics/v0.1|v0.]]"
created: 2026-02-25
updated: 2026-02-25
status: complete
---

# Feature: Camera System

## Overview

3D camera system with smooth transitions, keyboard/mouse controls, and multiple projection modes.

## Implementation Details

### Camera Features

**Movement & Control:**

- Distance, angle, and pitch smoothing with configurable agility
- Keyboard and mouse input (configurable enable/disable)
- Locking capabilities for angle, distance, and pitch
- Smooth floating-point transitions via SmoothFloat

**View Matrix:**

- Automatic view matrix calculation
- Change detection via CameraComparator
- View matrix update events

### Camera Management

**CameraManager:**

- System-based camera update management
- Integration with engine lifecycle
- Event dispatching for matrix updates

**CameraConfig:**

- Configuration flags for camera behavior
- Projection mode selection
- Serialization support via CameraSerializer

### Camera Events

**Dispatchers:**

- `ViewMatrixUpdated` - View matrix changed
- `ProjectionMatrixUpdated` - Projection matrix changed
