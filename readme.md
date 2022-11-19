# Getting started
## Cloning
This repo uses git submodules, so it must be cloned with --recursive:
```sh
git clone --recursive git@github.com:Benjman/uinta.git
```
## Build project
```sh
cmake -B build .     ## Configure the build directory
cmake --build build  ## Build the program in the build directory
```
## Run the programs
```sh
./build/demo
```
