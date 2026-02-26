---
tags: [feature]
epic: "[[Epics/v0.1]]"
created: 2026-03-01
updated: 2026-03-01
status: complete
priority: high
---

# Feature: OpenGL API Abstraction

## Overview

The Uinta Engine provides a comprehensive cross-platform OpenGL abstraction layer through the `OpenGLApi` interface.

The abstraction serves as the foundation for all graphics operations in the engine, providing type-safe wrappers around OpenGL function calls with extensive documentation derived from official OpenGL specifications.

## Architecture

### Core Components

**Platform Abstraction** (`src/platform/include/uinta/gl.h`)

- `OpenGLApi` - Pure virtual interface defining all OpenGL operations
- Platform-specific header inclusion:

**Uniform System** (`src/engine/include/uinta/uniform.h`)

- Type-safe uniform variable wrappers for shader parameters
- Automatic location caching and value change detection
- Support for scalar, vector, and matrix types
- Integration with GLM for math operations

## Usage Example

```cpp
// Platform initialization sets up the OpenGL context
// The OpenGLApi instance is provided by the platform layer

// Shader compilation
GLuint vertShader = gl->createShader(GL_VERTEX_SHADER);
const char* vertSource = /* ... */;
gl->shaderSource(vertShader, 1, &vertSource, nullptr);
gl->compileShader(vertShader);

// Check compilation status
GLint success;
gl->getShaderiv(vertShader, GL_COMPILE_STATUS, &success);
if (!success) {
  char infoLog[512];
  gl->getShaderInfoLog(vertShader, 512, nullptr, infoLog);
  // Handle error...
}

// Program linking
GLuint program = gl->createProgram();
gl->attachShader(program, vertShader);
gl->attachShader(program, fragShader);
gl->linkProgram(program);

// Uniform management
Shader myShader(program, gl);
Uniform3f colorUniform("uColor", &myShader);
UniformMatrix4fv modelMatrix("uModel", &myShader);

// Setting uniform values (automatically checks for changes)
colorUniform = glm::vec3(1.0f, 0.5f, 0.2f);
modelMatrix = glm::mat4(1.0f);

// Buffer setup
GLuint vbo, vao;
gl->genBuffers(1, &vbo);
gl->genVertexArrays(1, &vao);

gl->bindVertexArray(vao);
gl->bindBuffer(GL_ARRAY_BUFFER, vbo);
gl->bufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

// Vertex attributes
gl->vertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
gl->enableVertexAttribArray(0);

// Rendering
gl->clearColor(0.2f, 0.3f, 0.3f, 1.0f);
gl->clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
gl->enable(GL_DEPTH_TEST);
gl->useProgram(program);
gl->bindVertexArray(vao);
gl->drawArrays(GL_TRIANGLES, 0, 36);

// Cleanup
gl->deleteVertexArrays(1, &vao);
gl->deleteBuffers(1, &vbo);
gl->deleteProgram(program);
gl->deleteShader(vertShader);
gl->deleteShader(fragShader);
```
