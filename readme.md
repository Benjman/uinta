# Getting started

## Cloning

The Uinta Engine uses submodules, so it should be cloned with --recursive:

```sh
git clone --recurse-submodules --shallow-submodules git@github.com:Benjman/uinta.git
```

## Build project

```sh
cmake -B build . && make -j${nproc} --directory build
```

## Run the programs

```sh
./build/src/app/app
```
