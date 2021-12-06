#include <iostream>     // std::cout
//#include <vector>
//#include <algorithm>
#include <cstdlib>      // std::rand, std::srand
#include <time.h>       // time
#include <tbb/tbb.h>
//#include "oneapi/tbb/parallel_reduce.h"
//#include "oneapi/tbb/parallel_for.h"

using namespace std;
using namespace oneapi;



double GetMax(double x[], int n)
{
   auto max = tbb::parallel_reduce(
        tbb::blocked_range<int>(0, n),
        0.0,,
        /* lambda function
        [&](tbb::blocked_range<int> r, double total_acc){
            for (int i = r.begin();i<r.end();++i){
                total_acc += x[i];
            }
            return total_acc;
        }
      
    , */
        max<double>());
    return max;

}

int main(){

  time_t t; // lo empleo para generar luego la semilla para uso del rand()
  // Inicializo el generador de número aleatorio
  srand((unsigned) time(&t));
    
  //int size = 10000;
  //static vector<double> vector_in(size);
  int size = 10;
  double vector[] = {16,23,38,41,53,69,7543,8,99999,10};  

  tbb::tick_count t0 = tbb::tick_count::now();
  //generate(vector_in.begin(), vector_in.end(), rand);
  cout << "Time vector x: " << (tbb::tick_count::now()-t0).seconds() << "seconds" << endl;
  
    //double GetMax = GetMax(&vector_in[0], size);
  
    cout << "maximum: " << GetMax(&vector_in[0], size)  << endl;

  return 0;

 }