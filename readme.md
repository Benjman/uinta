# Uinta Engine

Another C++ game engine.

## What Exists

- Modular CMake build system with three components
- Fundamental type definitions (integer/float aliases)
- `Engine` class with game loop, tick/render stages, and event system
- `AppConfig` system for YAML-based configuration with typed getters/setters
- Platform layer with windowing, input polling, and monitor support (GLFW backend)
- Command-line argument processing (`ArgsProcessor`)
- File I/O utilities with search path resolution and RAII reader (`File`)
- OpenGL API abstraction with mock support for testing
- OpenGL rendering primitives: `Shader`, `Vbo`, `Vao`, `Uniform`, and `Texture` classes
- `BasicShader` with lighting, MVP matrices, and debug visualization modes (sway, UV, normals)
- `Mesh` class with vertex/element storage, primitive generation (Cube, Plane), and transform utilities
- OpenGL state guards: `CapabilityGuard`, `DepthTestGuard`, `CullFaceGuard`, `PolygonMode`
- `FrameManager` for frame timing and delta time management
- `Component` system with stage-typed components and thread-safe `ComponentManager`
- `Scene` system with layered scene management, lifecycle events, and debug UI
- `Input` system with key/mouse/scroll handling, event subscriptions, and input tokens
- `Camera` system with configurable camera, serialization, event handling, and debug UI
- `Font` system with TrueType loading and `TextMeshBuilder` for aligned, styled text rendering
- `LuaRuntime` plugin system with sol2 bindings, sandboxing, and hot reload support
- Code style enforcement (Google C++ style via clang-format, cpplint)
- Debug/Release build configuration

## Architecture

| Module   | Purpose                           |
|----------|-----------------------------------|
| platform | OS/windowing abstraction (GLFW)   |
| engine   | Core engine functionality         |
| app      | Demo/test application             |

## Building

> **Note:** This has only been tested on Arch Linux with the Wayland compositer. No guarantees for anything.

```sh
git clone --recurse-submodules --shallow-submodules git@github.com:Benjman/uinta.git
cmake -B build . && make -j$(nproc) --directory build
./build/src/app/app
```

### Required libraries

The following libraries are required to be available on your path prior to building:

- [Abseil](https://github.com/abseil/abseil-cpp)
- [assimp](https://github.com/assimp/assimp)
- [Dear ImGui](https://github.com/ocornut/imgui)
- [glm](https://github.com/g-truc/glm)
- [Lua 5.4](https://www.lua.org/ftp/lua-5.4.8.tar.gz)
- [Manifold](https://github.com/elalish/manifold)
- OpenGL headers
- [stb](https://github.com/nothings/stb)
- [yaml-cpp](https://github.com/jbeder/yaml-cpp)
