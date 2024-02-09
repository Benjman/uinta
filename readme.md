# Getting started

## Cloning

The Uinta Engine uses submodules, so it should be cloned with --recursive:

```sh
git clone --recurse-submodules --shallow-submodules git@github.com:Benjman/uinta.git
```

## Libraries

The following libraries are recommended to have been installed and available on
your system's path:

* [Abseil](https://github.com/abseil/abseil-cpp)
* [GLFW](https://github.com/glfw/glfw)

Any missing library not found on the path will be automatically downloaded and
linked to the project. Having the libraries on your path greatly reduces build
times.

## Build project

```sh
cmake -B build . && make -j${nproc} --directory build
```

## Run the programs

```sh
./build/src/app/app
```
