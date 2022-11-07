# libr
[![Build and test](https://github.com/rootmos/libr/actions/workflows/test.yaml/badge.svg)](https://github.com/rootmos/libr/actions/workflows/test.yaml)

Opinionated public domain C snippets and helpers,
bundled as a [single-file library](https://github.com/nothings/stb).

## Usage
Select a few modules, for instance `fail` and `logging`:
```shell
@include ".usage.bundle.sh"
```
Then for example `hello.c`:
```c
@include "hello.c"
```
which when built and run:
```shell
@include ".usage.build.sh"
```
says hello:
```
@include ".usage.output.txt"
```

## Modules
@include "modules.list.md"
