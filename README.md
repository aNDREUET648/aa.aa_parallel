21741 - Arquitectures Avançades

Basándonos en la la biblioteca [OneAPI Threading Building Blocks (oneTBB)] ()  es una biblioteca basada en plantillas para C++ desarrollada por Intel para facilitar la escritura de programas que explotan las capacidades de paralelismo de los procesadores con arquitectura multinúcleo.

una biblioteca de rendimiento flexible que simplifica el trabajo de agregar paralelismo a aplicaciones complejas, incluso si no es un experto en subprocesos.


## Using oneTBB as a dependency

This example demonstrates how to use oneTBB as a dependency within a Bazel project.

The following file structure is assumed:

```
example
├── .bazelrc
├── BUILD.bazel
├── main.cpp
└── WORKSPACE.bazel
```

_WORKSPACE.bazel_:
```python
load("@bazel_tools//tools/build_defs/repo:git.bzl", "git_repository")

git_repository(
    name = "oneTBB",
    branch = "master",
    remote = "https://github.com/oneapi-src/oneTBB/",
)
```

In the *WORKSPACE* file, the oneTBB GitHub* repository is fetched. 

_BUILD.bazel_:

```python
cc_binary(
    name = "Demo",
    srcs = ["main.cpp"],
    deps = ["@oneTBB//:tbb"],
)
```

The *BUILD* file defines a binary named `Demo` that has a dependency to oneTBB.

_main.cpp_:

```c++
#include "oneapi/tbb/version.h"

#include <iostream>

int main() {
    std::cout << "Hello from oneTBB "
              << TBB_VERSION_MAJOR << "."
              << TBB_VERSION_MINOR << "."
              << TBB_VERSION_PATCH
              << "!" << std::endl;

    return 0;
}
```

The expected output of this program is the current version of oneTBB.

Switch to the folder with the files created earlier and run the binary with `bazel run //:Demo`.

## Build oneTBB using Bazel

Run ```bazel build //...``` in the oneTBB root directory.
