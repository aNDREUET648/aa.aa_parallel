/*
          Uso del parallel_scan:

          Filtrar un vector por un criterio determinado
          En este caso buscaremos los elementos del vector que cumplan un CRITERIO

*/
#include <iostream>
#include <cstdlib>      // std::rand, std::srand
#include <algorithm>    // std::generate
#include <tbb/tbb.h>
#include "oneapi/tbb/blocked_range.h"
#include "oneapi/tbb/parallel_for.h"

using namespace std;
using namespace oneapi;


/*
          El mapeo devolverá un vector del tamaño del de entrada
          con un '1' (true) en la posición si se cumple el CRITERIO
          que en nuestro caso es >= 10
          y un '0' (false) en el resto de posiciones

          - a será el número por el que vamos a comprobar
          - x[] es el vector de entrada (vector_in)
          - n es el tamaño del vector

          parallel_for en forma funcional (con expresiones lambda)
*/
vector<int> doMAP(int a,int x[], int n)
{
    vector<int> out(n);

    tbb::parallel_for(
        tbb::blocked_range<int>(0, n),
        // lambda function
        [&](tbb::blocked_range<int> r) {
            for (auto i = r.begin(); i != r.end(); ++i) {
                out[i] = x[i]>=a;
            }
        }

    );
    return out;
}

/*
          El escaneo devolverá dos elementos:
          - vector (vector_salida) con las posiciones que tienen los
          elementos 'mapeados' (que cumplen el CRITERIO).
          Este vector es INCLUSIVO (del tipo algoritmo Hilliss & Steele)
          - int total_sum con la suma total de los que hay      

          Los tipos pre_scan_tag y final_scan_tag son 'dummy types' y se usan
          en conjunción con parallel_scan para distinguir las fases del mismo

          bool is_final_scan() devuelve: true para un final_scan_tag
                                          false en cualquier otro caso

          parallel_scan en forma funcional (con expresiones lambda)
*/
int doSCAN(int vector_salida[], const int vector_entrada[], int n)
{
    int total_sum = tbb::parallel_scan(
        tbb::blocked_range<int>(0, n), //range
        0, // identidad
        [&](tbb::blocked_range<int> r, int sum, bool is_final_scan) -> int {        
            int tmp = sum;
            for (int i = r.begin(); i < r.end(); ++i) {
                tmp = tmp + vector_entrada[i];
                if (is_final_scan)
                    vector_salida[i] = tmp;
            }
            return tmp; // sum =tmp; return sum;
        },
        [&]( int left, int right ) {      //   Combine using the lambda function
            return left + right;
        }
    );
    cout << "hemos llegado aqui" << endl;
    return total_sum;
}

/*
          El join es la última parte del problema, en este caso tenemos:
          - vector del mapeo (vector_bool == bolMatch)
          - vector del escaneo (ixMatch)
          - vector original (vector_in == x)
          Y esto nos devolverá:
          - vector de los resultados filtrados (filter_results == out)

          parallel_for en forma funcional (con expresiones lambda)

*/
void doMAPFilter(int bolMatch[], int ixMatch[],int x[], int out[], int n)
{
    tbb::parallel_for(
        tbb::blocked_range<int>(0, n),
        // lambda function
        [&](tbb::blocked_range<int> r) {
            for (auto i = r.begin(); i < r.end(); ++i) {
                if (bolMatch[i]){
                    out[ixMatch[i]-1] = x[i];    
                }
            }
        }
    );
}


int main(){

    static int filtro = 10;        // elemento por el que se va a filtrar
    static vector<int> vector_in{7,1,10,13,0,15,20,-1};
    static int tamany = vector_in.size();


    /*

                  versión para vector más grande y con número aleatorios

    int tamany = 100;
    static int filtro = 1978785360;        // elemento por el que se va a filtrar
    static vector<int> vector_in(tamany);
    generate(vector_in.begin(), vector_in.end(), rand);
    */



    cout << "Vector de entrada: "<< endl;
    for (int i: vector_in){
        cout << i << ',';
    }
    cout << endl;

    // inicializamos contador de tiempo para ver que tarda en realizar
    // las 3 'operaciones' MAP, SCAN y JOIN
    tbb::tick_count t0 = tbb::tick_count::now();


  
    //            MAP 
    vector<int> vector_bool = doMAP(filtro,&vector_in[0], tamany);
  
    cout << "Vector mapeado de booleanos: (ocurrencias)"<< endl;
    for (int i: vector_bool){
        cout << i << ',';
    }
    cout << endl;



    //            SCAN
    vector<int> ixMatch(tamany); // este vector será devuelto con los valores que necesitamos
    int cuantos_items = doSCAN(&ixMatch[0], &vector_bool[0],  tamany); //get index order

    cout << "Vector escaneado: (posiciones y total de items: )" << cuantos_items << endl;
    for (int i: ixMatch){
        cout << i << ',';
    }
    cout << endl;



    //            JOIN
    vector<int> filtered_results(cuantos_items);  // vector con los items que cumplen el CRITERIO
    doMAPFilter(&vector_bool[0],&ixMatch[0],&vector_in[0],&filtered_results[0], tamany);

    cout << "Vector de salida (filtrado): " << endl;
    for (int i: filtered_results){
        cout << i << ',';
    }
    cout << endl;


    cout << "\nTiempo de ejecución: " << (tbb::tick_count::now()-t0).seconds() << "segundos" << endl;
    return 0;
}