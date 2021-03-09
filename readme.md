### Primary goals

- Render a terrain (Perhaps primitive cubes, or simple terrain heightmap (Perf test the two?))
- Render basic flora; Shrubs, flowers, etc.?
- Measure how large of an area can be rendered in 1/60th of a second (1 FPS) on both android and desktop.

### Architecture plan

Build engine features as independent libraries. For example, the Shader system doesn't depend on anything other than OpenGL, so it
could be implemented as its own static library.

### Dependencies

Requirement | Library
----- | -----
Asset importer | [assimp](https://github.com/assimp/assimp)
Audio | *not implemented v1*
ECS | [EnTT](https://github.com/skypjack/entt)
Font rasterizing | [stb_truetype.h](https://github.com/nothings/stb/blob/master/stb_truetype.h)
Graphics API | [OpenGL ES 3.0](https://www.khronos.org/registry/OpenGL-Refpages/es3/)
Input Manager | [gainput](https://gainput.johanneskuhlmann.de/)
Math | [glm](https://github.com/g-truc/glm)
Windowing | GLFW and Android

### Getting Started
Running `./configure` from the Uinta base directory should download and initialize all dependencies. Then doing an
out-of-tree cmake build, and make should be all that is needed to create the executable.
