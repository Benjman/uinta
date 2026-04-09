# Uinta Engine

Another C++ game engine.

## What Exists

- Modular CMake build system with three components
- Fundamental type definitions (integer/float aliases)
- `Engine` class with game loop, tick/render stages, and event system
- Platform layer with windowing, input polling, and monitor support (GLFW backend)
- Command-line argument processing (`ArgsProcessor`)
- OpenGL API abstraction with mock support for testing
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
- [glm](https://github.com/g-truc/glm)
- OpenGL headers
