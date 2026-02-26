---
tags: [feature]
epic: "[[Epics/v0.1]]"
created: 2026-02-25
updated: 2026-02-25
status: in-progress
---

# Feature: Hexagonal Grid System

## Overview

Complete hexagonal grid system with axial coordinates, multiple layout patterns, and mesh generation.

## Implementation Details

### Hex Coordinate System

**Hex Class:**

- Axial coordinates (q, r)
- Direction and diagonal lookups
- Neighbor calculation (6 neighbors)
- Distance calculation between hexes
- Center point calculation
- Point generation with fill percentage

**Coordinate Math:**

- Addition and subtraction
- Multiplication by scalar
- Distance via axial coordinates
- Equality and comparison operators

### Hex Group Management

**HexGroup Features:**

- Ring generation (hexes at specific distance)
- Spiral generation (filled hexes up to distance)
- Rectangle layouts
- Rhombus layouts
- Per-hexagon coloring
- Flat-top and pointy-top orientations

**World Conversion:**

- Hex to world point conversion
- World point to hex conversion
- Configurable hex size

### Mesh Generation

**Hexagon Meshes:**

- `Mesh::Hexagon::Fill()` - Filled hexagon mesh
- `Mesh::Hexagon::Outline()` - Hexagon outline mesh
- Per-hex coloring support
- Batch generation for groups

## Use Cases

- Hex-based strategy games
- Tile-based world generation
- Grid-based pathfinding
- Board game implementations
