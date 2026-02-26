---
tags: [task]
feature: "[[Features/OpenGL]]"
created: 2026-03-01
updated: 2026-03-01
status: planned
priority: low
estimate: low
---

# Task: Refactor Uniform Value Caching

## Goal

Move uniform value caching from scene-level to the Uniform class itself, improving performance and efficiency by streamlining uniform management and GPU updates.

## Context

Currently, when a scene receives an event to update the projection matrix in a shader, we set flags to mark the matrix as dirty and store it in a local variable for that scene. During rendering, we check these flags and copy the matrix from the local variable to the GPU if needed.

This approach scatters the caching logic across scenes rather than centralizing it in the Uniform class where it logically belongs.

## Acceptance Criteria

- [ ] Modify Uniform class to maintain its own local CPU-side copy of the value
- [ ] Implement `void operator=()` override on each uniform type to store values locally
- [ ] Implement `flush()` function on each uniform to push cached values to GPU
- [ ] Update flags to only indicate when cached values need flushing to GPU
- [ ] Remove scene-level caching of uniform values
- [ ] Update render loop to call uniform flush when needed
- [ ] Verify performance improvement compared to current implementation
- [ ] Update tests to cover new caching behavior

## Implementation Notes

**Design approach:**

- Uniform class becomes responsible for its own value lifecycle
- CPU-side cache is maintained per-uniform instance
- Dirty flags indicate GPU sync needed, not value storage

**Technical considerations:**

- Each uniform type will need custom `operator=()` implementation
- `flush()` function handles GPU transfer when dirty flag is set
- Need to ensure thread-safety if uniforms are updated from multiple contexts
- Consider memory implications of per-uniform caching vs scene-level caching

**Migration strategy:**

- Start with projection matrix uniforms as proof of concept
- Gradually migrate other uniform types
- Maintain backwards compatibility during transition if needed
