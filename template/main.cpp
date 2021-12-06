#include "oneapi/tbb/parallel_invoke.h"

#include <iostream>

int main(){
 oneapi::tbb::parallel_invoke(
   [](){std::cout << "Hello " << std::endl;},
   [](){std::cout << "TBB " << std::endl;}
 );
   return 0;
}
