# Getting started

## Cloning

The Uinta Engine uses submodules, so it should be cloned with --recursive:

```sh
git clone --recurse-submodules --shallow-submodules git@github.com:Benjman/uinta.git
```

## Libraries

It's recommended to include these libraries in your system's path, although it's
not mandatory. If a library isn't found on the path, it will be automatically
downloaded and linked to the project. However, having the libraries on your path
can greatly speed up the build times.

* [Abseil](https://github.com/abseil/abseil-cpp)
* [assimp](https://github.com/assimp/assimp)
* [Dear ImGui](https://github.com/ocornut/imgui)
* [eventpp](https://github.com/wqking/eventpp)
* [GLFW](https://github.com/glfw/glfw)
* [glm](https://github.com/g-truc/glm)

## Build project

```sh
cmake -B build . && make -j${nproc} --directory build
```

## Run the programs

```sh
./build/src/app/app
```
