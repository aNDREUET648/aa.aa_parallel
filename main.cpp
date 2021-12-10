/*
    La expresión parallel_invokes evalua varias funciones en paralelo.

    En este caso tenemos 3 funciones lambda que imprimen 3 textos:
      - "Hello"
      - "TBB version"
      - '2021.6.0'!  (en mi caso es la version que me devuelve)

    El orden de impresión no tiene porque ser secuencial, de hecho no lo será
    
*/

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
