# Cloning
This repo uses git submodules, so it must be cloned with --recursive:
> git clone --recursive git@github.com:Benjman/uinta.git


# Building
## Install dependencies
### I use Arch BTW
Install the following packages:
* glfw-wayland
* gtest
* glm

### MacOS (not quite working yet, need to add glm from homebrew to the include path)
> brew install cmake

> brew install googletest

> brew install glfw

> brew install glm

## Run cmake:
> cmake -B build .     # Configure the build directory

> cmake --build build  # Build the program in the build directory

## Run the programs
> ./build/camera2d

> ./build/camera3d



