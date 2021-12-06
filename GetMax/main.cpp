/*
        obtener el valor máximo de un vector
        
        generamos aleatoriamente un vector de tipo long
        de tamaño 'size' y nos devuelve el valor del más grande

        paralle_reduce
*/


#include <iostream>     // std::cout
//#include <vector>
//#include <algorithm>
#include <iomanip>      // std::setprecision
#include <cstdlib>      // std::rand, std::srand
#include <time.h>       // time
#include <tbb/tbb.h>
//#include "oneapi/tbb/parallel_reduce.h"
//#include "oneapi/tbb/parallel_for.h"

using namespace std;
using namespace oneapi;



long GetMax(long x[], int n)
{
    long maximo = x[0];
    return tbb::parallel_reduce(
        tbb::blocked_range<int>(0, n),
        0,
        [&](const tbb::blocked_range<int> r, int init){
            for (int i=r.begin(); i<r.end(); ++i)
            {
                if ( x[i] > maximo) maximo = x[i];
            }
            //cout << maximo << '\n';
            return maximo;
        },
        [&](long lhs, long rhs) -> long {
          cout << "final de la iteración max(lhs,rhs)" << endl;
          return max(lhs, rhs);
        }
    );
}

int main(){

  //int size = 10;
  //long vector_in[] = {16.4,943.23,38.32,41,53,69,75,8,943.38,10};


  srand(time(NULL));
  
  long resultado = 0; 
  
  int size = 1000;
  static vector<long> vector_in(size);
  
  cout << "Tamaño de vector_in[]: " << size << " elementos" << endl;
  
  tbb::tick_count t0 = tbb::tick_count::now();
  generate(vector_in.begin(), vector_in.end(), rand);
  cout << "Tiempo de crear el vector: " << (tbb::tick_count::now()-t0).seconds() << "segundos" << endl;
  
    /* visualizar el contenido del vector
    for (const auto &item : vector_in) {
        cout << item << "; ";
        cout <<  endl;
    }
    */

  tbb::tick_count t1 = tbb::tick_count::now();
  resultado = GetMax(&vector_in[0], size);
  cout << "Time en encontrarlo: " << (tbb::tick_count::now()-t1).seconds() << "segundos" << endl;


  cout << "Valor máximo: " << setprecision(5) << fixed << resultado  << endl;
  return 0;
 }