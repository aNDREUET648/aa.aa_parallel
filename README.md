# 21741 - Arquitectures Avançades - Programación paralela

## Introducción

  aa.aa_parallel incluye una serie de actividades (cada una en un directorio diferente) de programación paralela que se corresponden con las actividades propuestas a lo largo del curso. 
  En cada directorio se destacará el uso de las funciones principales de la biblioteca oneTBB.
    
 ## Estructura 
 
 ```
aa.aa_parallel
├── helloworld          Primera aplicación por excelencia siempre que se comienza (parallel_invoke)
├── parallel_for        Actividad 1. Sumar 1 a cada elemento de un vector (parallel_for)
├── parallel_reduce     Actividad 2. Sumar los 100 primeros números de 1 a 101 (parallel_reduce)
├── GetMax              Actividad 3. Obtener el valor máximo de un vector (parallel_reduce)
├── parallel_scan       Actividad 4. Filtrar un vector por un criterio determinado (MAP,SCAN & JOIN)
├── template            Plantilla necesaria para realizar las actividades (Ver Requisitos más abajo)
└── README.md           (este archivo)
```
 

## Requisitos

   Las actividades de programación paralela se realizarán en C++, apoyándome del uso de dos herramientas para facilitar la escritura de los programas:  
   
   * La biblioteca [OneAPI Threading Building Blocks (oneTBB)](https://oneapi-src.github.io/oneTBB/) basada en plantillas para C++ desarrollada por Intel que explotan las capacidades de paralelismo de los procesadores con arquitectura multinúcleo.
   * La herramienta de software gratuita [Bazel](https://bazel.build/) para la automatización de la construcción y pruebas de software similar [Make](https://www.gnu.org/software/make), [Maven](https://maven.apache.org/), o [Gradle](https://gradle.org/).


### Empleando oneTBB como una dependencia dentro de un proyecto Bazel

El enfoque estándar de Bazel para manejar las bibliotecas de terceros es el enlace estático. Por lo que los pasos a seguir son:

Definiremos una plantilla por defecto con esta estructura:

```
template
├── BUILD.bazel
├── main.cpp
└── WORKSPACE.bazel
```
Con el siguiente contenido en cada uno de los archivos

_WORKSPACE.bazel_: En este archivo el repositorio oneTBB de GitHub* es 'traido' (fetched)
```python
load("@bazel_tools//tools/build_defs/repo:git.bzl", "git_repository")

git_repository(
    name = "oneTBB",
    branch = "master",
    remote = "https://github.com/oneapi-src/oneTBB/",
)
```

_BUILD.bazel_: Este archivo define un binario llamado `Demo` que tiene una dependencia con oneTBB.

```python
cc_binary(
    name = "Demo",
    srcs = ["main.cpp"],
    deps = ["@oneTBB//:tbb"],
)
```

_main.cpp_: Aquí es donde escribiré mi programa principal.

```c++
#include "oneapi/tbb/parallel_invoke.h"
#include "oneapi/tbb/version.h"   // TBB_VERSION_MAJOR, MINOR & PATCH

#include <iostream>


int main(){
 oneapi::tbb::parallel_invoke(

   [](){std::cout << "TBB version" << std::endl;},
   [](){std::cout << TBB_VERSION_MAJOR << "."
                  << TBB_VERSION_MINOR << "."
                  << TBB_VERSION_PATCH
                  << "!" << std::endl;},
   [](){std::cout << "Hello World!!" << std::endl;},
 );
   return 0;
}
```

Vas al directorio con los archivos creados antes y ejecutas el binario con `bazel run //:Demo`.
El resultado se ejecutará directamente.
