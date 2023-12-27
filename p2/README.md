# LemonDB 

## Introduction

This is a multi-thread database by Lemonion. Inc. 

For more information, see official documentation: p2.pdf in https://jicanvas.com/courses/482/files/folder/projects canvas page. (Download link: https://jicanvas.com/files/168565/download?download_frd=1)

## Compilation

```bash
cmake -DCMAKE_C_COMPILER=/usr/bin/clang -DCMAKE_CXX_COMPILER=/usr/bin/clang++-16 -DCMAKE_EXPORT_COMPILE_COMMANDS=ON ../src
cmake --build . -- -j8
```

## Testing

The test cases are under `./test` folder. The test cases also the program arguments are named as `test.query`.

## Recovered Files

[![Build Status](https://focs.ji.sjtu.edu.cn:2222/api/badges/ece482-23fa/p2team-02/status.svg)](https://focs.ji.sjtu.edu.cn:2222/ece482-23fa/p2team-02)

## Files Structure

- ./src
  Contains the source code for an old version of lemnonDB.
  The code was recovered from crash site. As far as we know the
  original developer used CMake as their building system.

- ./bin
  Contains the lastest binary that survived the crash.

- ./db
  Contains sample database files.

- ./sample
  Sample inputs and outputs

## Developer Quick Start

See INSTALL.md for instructions on building from source.

`ClangFormat` and `EditorConfig` are used to format codes.

Hint on using `ClangFormat`:
`find . -name "*.cpp" -o -name "*.h" | sed 's| |\\ |g' | xargs clang-format -i`

And make sure your code editor has `EditorConfig` support.

## Performance

The time is around 50% to 100%, depending on specific queries.

## Analysis of Problems

The lowest queries are `SELECT` and `DUPLICATE`.

## Copyright

Lemonion Inc. 2018
