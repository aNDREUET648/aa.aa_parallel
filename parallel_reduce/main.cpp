/*
                Sumar los 100 primeros números de 1 a 101

                parallel_reduce 


*/#include <oneapi/tbb/info.h>
#include <oneapi/tbb/parallel_reduce.h>
#include <iostream>     // std::cout



// A diferencia del parallel_for, necesitamos definir
// el blocked_range.h para su uso en el parallel_reduce
// ya que por defecto no lo incluye
#include <oneapi/tbb/blocked_range.h>

using namespace std;
using namespace oneapi::tbb;

int main() {

  /*
          I have to use std::mutex to ensure that each worker thread was able to print 
          to the screen without interrupting any other thread.

          Making use of the lock() and unlock() function calls
    */
        mutex m;


    // Get the default number of threads
    int num_threads = info::default_concurrency();
        cout << "Concurrencia por defecto: " 
             << num_threads 
             << "\n\n" << endl;
    /*
        En esta actividad el parallel_reduce nos permitirá sumar los 100 primeros números
        desde 1 a 101 

        auto sum = tbb::parallel_reduce( range, identity_value, kernel, reduction_function )
        
        range: rango de valores con los que vamos a iterar
        identity_value: el elemento identidad para la reducción. 
                        en nuestro caso el elemento identidad (neutro) de la suma es 0
        kernel: es la función lambda [&] que realiza las iteraciones de la reducción.

                [&](oneapi::tbb::blocked_range<int> const& r, int init) -> int {
                    for (int v = r.begin(); v != r.end(); ++v)
                        init += v;
                }
        reduction_function: vuelve a ser otra función lambda [=]
                          donde vamos sumando los valores de cada lado resultantes
                          de cada iteración.
    */
    int sum = parallel_reduce(blocked_range<int>(1,101), 0,
        [&](blocked_range<int> const& r, int init) -> int {
            /*
              Podemos apreciar que al imprimirse no lo hará secuencialmente
              se ejecutarán los procesos que estén ejecutándose en ese momento
              pero para asegurarnos la correcta impresión me valgo de la primitiva 
              de sincronización y proteger los datos compartidos que están siendo 
              accedidos simultáneamente por múltiples hilos (threads).
            */
            m.lock();
            cout << "r.Begin:" << r.begin() 
                 << "\tr.End:" << r.end() 
                 << '\n';  
            m.unlock();

            for (int v = r.begin(); v != r.end(); ++v) {
               init += v;
            }
            return init;
          },
            // en vez de la función lambda podríamos haber usado también
            // std::plus<double>()
            [=](int lhs, int rhs) -> int {
               return lhs + rhs;
            }
        );
    //
    // sum es el resultado de sumar el último valor por la izquierda (lhs)
    // y el último valor por la derecha (rhs)
    //
    cout << "Sum " << sum << endl;
    return 0;
}