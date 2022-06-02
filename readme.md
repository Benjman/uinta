# Versions
## v0.1
* Rasterized GPU based font rendering
* Complex model loading of `.obj` files.
	* Currently parsing vertices, normals, and uv coordinates.
* Quadtree spacial partitioning
* Ray picking - getting world coordinates from screen cursor
* Target camera with pawn, yaw, and zoom
* ImGUI tools
# Getting started
## Cloning
This repo uses git submodules, so it must be cloned with --recursive:
```sh
git clone --recursive git@github.com:Benjman/uinta.git
```
## Dependencies
Install the following packages:
* glfw-wayland
* gtest
```sh
brew install cmake
brew install googletest
brew install glfw
```
## Build project
```sh
cmake -B build .     ## Configure the build directory
cmake --build build  ## Build the program in the build directory
```
## Run the programs
```sh
./build/camera3d
```
