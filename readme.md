# Uinta Engine

Another C++ game engine.

## What Exists

- Modular CMake build system with three components
- Fundamental type definitions (integer/float aliases)
- `Engine` class with game loop, tick/render stages, and event system
- Platform layer with windowing, input polling, and monitor support (GLFW backend)
- Command-line argument processing (`ArgsProcessor`)
- File I/O utilities with search path resolution and RAII reader (`File`)
- OpenGL API abstraction with mock support for testing
- Code style enforcement (Google C++ style via clang-format, cpplint)
- Debug/Release build configuration

## Architecture

| Module   | Purpose                           |
|----------|-----------------------------------|
| platform | OS/windowing abstraction (GLFW)   |
| engine   | Core engine functionality         |

The demo application (`app`) lives in the sibling `uinta-game` project, which consumes this repo as an installed `find_package(uinta)` package rather than building in-tree.

## Building

> **Note:** This has only been tested on Arch Linux with the Wayland compositer. No guarantees for anything.

```sh
git clone --recurse-submodules --shallow-submodules git@github.com:Benjman/uinta.git
cmake -B build . -DCMAKE_INSTALL_PREFIX=<prefix> && make -j$(nproc) --directory build
cmake --install build
```

To run the demo, build the `uinta-game` project against the installed
package: `cmake -B build . -DCMAKE_PREFIX_PATH=<prefix>` from within
`uinta-game`, then build and run its `app` target.

### Required libraries

The following libraries are required to be available on your path prior to building:

- [Abseil](https://github.com/abseil/abseil-cpp)
- [glm](https://github.com/g-truc/glm)
- OpenGL headers
