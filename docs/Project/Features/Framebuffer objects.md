---
tags: [feature]
epic: "[[Epics/v0.1]]"
created: 2026-02-25
updated: 2026-02-25
status: complete
---

# Feature: Framebuffer Objects

## Overview

Framebuffer Object (FBO) support for render-to-texture capabilities with color and depth attachments.

## Implementation Details

### FBO Features

**Framebuffer:**

- Color texture attachments
- Depth renderbuffer attachments
- Depth-stencil attachment support
- Framebuffer completeness checking

**Use Cases:**

- Off-screen rendering
- Post-processing effects
- Shadow mapping
- Reflection/refraction
- Multi-pass rendering

### RAII Management

**FboGuard:**

- Automatic framebuffer activation
- Automatic restoration of previous framebuffer
- Exception-safe binding
