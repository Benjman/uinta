---
tags: [feature]
epic: "[[Epics/v0.1]]"
created: 2026-03-01
updated: 2026-03-01
status: planned
priority: medium
---

# Feature: Particle System

## Overview

A flexible and performant particle system for rendering large numbers of dynamic particles. Supports various particle behaviors including physics simulation, color gradients, size changes, and texture-based rendering. The system uses GPU instancing for efficient rendering of thousands of particles.

## Requirements

- [ ] Particle emitter with configurable emission patterns
- [ ] Particle lifecycle management (spawn, update, death)
- [ ] Physics simulation (velocity, acceleration, drag)
- [ ] Color gradients over particle lifetime
- [ ] Size changes over particle lifetime
- [ ] Texture atlas support for varied particle sprites
- [ ] GPU instancing for efficient rendering
- [ ] Multiple emitter types (point, cone, sphere, box)
- [ ] Particle pooling for memory efficiency
- [ ] Collision detection (optional)
- [ ] Sorting for alpha blending (optional)

## Implementation Details

### Core Classes

**Particle:**
- Position, velocity, acceleration
- Lifetime tracking (age, max lifetime)
- Color (current and gradient support)
- Size (current and scale over lifetime)
- Texture coordinates/sprite index
- Rotation and angular velocity
- Active state flag

**ParticleEmitter:**
- Emission rate and burst modes
- Spawn patterns (point, cone, sphere, box, directional)
- Initial particle properties (velocity range, lifetime range, size range, color range)
- Continuous or one-shot emission
- Transform-based positioning
- Enable/disable state

**ParticleSystem:**
- Manages multiple emitters
- Updates all active particles
- Culls dead particles
- Handles particle pooling
- Prepares instance data for rendering
- Integration with scene graph

**ParticleRenderer:**
- Instanced rendering using vertex attributes
- Billboard particles (always face camera)
- Additive or alpha blending modes
- Texture atlas support
- Custom particle shader
- Depth sorting (optional, for alpha blending)

### Particle Update Pipeline

1. **Emission Phase:** Spawn new particles based on emission rate
2. **Physics Phase:** Apply velocity, acceleration, drag
3. **Lifetime Phase:** Age particles, evaluate curves
4. **Death Phase:** Deactivate particles past lifetime
5. **Sorting Phase:** Sort by depth if needed for blending
6. **Rendering Phase:** Update instance buffer, draw batched particles

### Performance Considerations

- Use object pooling to avoid allocations
- Batch particles by texture/material
- Use GPU instancing for draw calls
- Limit active particle count with budget system
- Spatial partitioning for collision detection
- Fixed timestep updates for consistency

### Configuration Options

**Per-Emitter Settings:**
- Max particles
- Emission rate (particles/second)
- Particle lifetime range
- Initial velocity range and direction
- Color gradient (over lifetime)
- Size curve (over lifetime)
- Texture/sprite selection
- Physics parameters (gravity, drag)
- Spawn volume shape and size

## Usage Example

```cpp
// Create particle system
ParticleSystem particleSystem;

// Create a fire emitter
ParticleEmitter fireEmitter;
fireEmitter.setEmissionRate(50.0f);  // 50 particles per second
fireEmitter.setMaxParticles(500);
fireEmitter.setLifetimeRange(1.0f, 2.0f);
fireEmitter.setSpawnShape(EmitterShape::Cone, glm::vec3(0, 1, 0), 15.0f);
fireEmitter.setVelocityRange(glm::vec3(-0.5f, 2.0f, -0.5f), 
                              glm::vec3(0.5f, 4.0f, 0.5f));

// Configure color gradient (yellow to red to transparent)
fireEmitter.setColorGradient({
    {0.0f, glm::vec4(1.0f, 1.0f, 0.0f, 1.0f)},  // Yellow
    {0.5f, glm::vec4(1.0f, 0.3f, 0.0f, 1.0f)},  // Orange-red
    {1.0f, glm::vec4(0.5f, 0.0f, 0.0f, 0.0f)}   // Transparent
});

// Configure size curve (start small, grow, then shrink)
fireEmitter.setSizeCurve({
    {0.0f, 0.2f},
    {0.3f, 1.0f},
    {1.0f, 0.1f}
});

// Set physics
fireEmitter.setGravity(glm::vec3(0.0f, -0.5f, 0.0f));
fireEmitter.setDrag(0.1f);

// Add to system
particleSystem.addEmitter(&fireEmitter);

// In game loop
void update(float dt) {
    particleSystem.update(dt);
}

void render(const Camera& camera) {
    ParticleRenderer::render(particleSystem, camera);
}

// One-shot burst (explosion, impact, etc.)
ParticleEmitter explosionEmitter;
explosionEmitter.setEmissionMode(EmissionMode::Burst);
explosionEmitter.setBurstCount(100);
explosionEmitter.setSpawnShape(EmitterShape::Sphere, glm::vec3(0, 0, 0), 1.0f);
explosionEmitter.emit();  // Trigger burst
```

## Dependencies

- Shader system (for particle rendering shader)
- Texture system (for particle sprites/atlases)
- Camera system (for billboarding)
- OpenGL abstraction (VBO, instancing)
- Time/delta time from engine core

## Future Enhancements

- GPU-based particle simulation (compute shaders)
- Particle collision with scene geometry
- Soft particles (depth-buffer based)
- Particle trails/ribbons
- Force fields and attractors
- Sub-emitters (particles that spawn particles)
- Texture animation/flipbook
- Mesh particles (non-billboard)
- LOD system for distant particle systems
