# Uinta Engine

Another C++ game engine.

## What Exists

- Modular CMake build system with three components
- Fundamental type definitions (integer/float aliases)
- Placeholder `Engine` class
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
