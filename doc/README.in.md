# libr
[![Build and test](https://github.com/rootmos/libr/actions/workflows/test.yaml/badge.svg)](https://github.com/rootmos/libr/actions/workflows/test.yaml)

Opinionated public domain C snippets and helpers,
bundled as a [single-file library](https://github.com/nothings/stb).

## Usage
Select a few modules: from the list below or run `bundle -l` to list the
available modules.
For instance: `fail` and `logging`:
```shell
@include ".usage.bundle.sh"
```
Then `hello.c` can check for errors and do logging:
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
