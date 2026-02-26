---
tags: [feature]
epic: "[[Epics/v0.1]]"
created: 2026-02-25
updated: 2026-02-25
status: complete
---

# Feature: Shader System

## Overview

Comprehensive shader system with compilation, linking, uniform management, and specialized shader implementations.

## Requirements

- [x] Base Shader class with program management
- [x] Shader compilation from source
- [x] Attribute and uniform linking
- [x] BasicShader with full lighting and transformation support
- [x] TextShader for font rendering
- [x] Uniform types for all GLSL data types
- [x] ShaderGuard for RAII activation

## Implementation Details

### Shader Classes

**Base Shader:**

- Vertex and fragment shader compilation
- Program linking and validation
- Attribute location binding
- Uniform location caching

**BasicShader:**

- Model, View, Projection matrices
- Lighting support:
  - Ambient strength
  - Diffuse minimum
  - Light color
  - Light direction
- Material properties
- Time uniform for animations
- Feature flags (sway, UV mapping, normals)

**TextShader:**

- Atlas-based text rendering
- Texture sampler uniform
- Projection matrix support

### Uniform System

**Supported Types:**

- Float vectors: `Uniform1f`, `Uniform2f`, `Uniform3f`, `Uniform4f`
- Integer vectors: `Uniform1i`, `Uniform2i`, `Uniform3i`, `Uniform4i`
- Matrices: `UniformMat2`, `UniformMat3`, `UniformMat4`, `UniformMat2x3`, `UniformMat3x2`, `UniformMat2x4`, `UniformMat4x2`, `UniformMat3x4`, `UniformMat4x3`

### RAII Management

**ShaderGuard:**

- Automatic shader activation on construction
- Automatic deactivation on destruction
- Exception-safe resource management

## Usage Example

```cpp
BasicShader shader;
ShaderGuard guard(&shader);

shader.model.set(modelMatrix);
shader.view.set(viewMatrix);
shader.projection.set(projectionMatrix);
shader.lightColor.set(glm::vec3(1.0f));
shader.lightDir.set(glm::vec3(0.0f, -1.0f, 0.0f));

// Draw operations here
```
