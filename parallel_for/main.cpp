/*
			 Parallel programming with TBB
		Activity with TBB oneapi::tbb::parallel_for()
		to add 1 to vector[] and return it in ouput[]
*/
#include <oneapi/tbb/parallel_for.h>
#include <iostream>


using namespace std;  	// makes names from std visible without std::

int main(){

	int vector[] = {1,2,3,4,5,6,7,8,9,10};
	int output[10];
	/*
			tbb::parallel_for( range, kernel );
			where range is the range of values over which the loop will iterate, 
			and kernel is a lambda function that will be called for subsets of the range 
			of values of the loop.

			In our case, the range is tbb::blocked_range<int>(0,10). 
			This creates a TBB range object that iterates between 0 and 10.

			The kernel function is:
			[&](tbb::blocked_range<int> r){
				for (auto i = r.begin(); i != r.end(); ++i){
						output[i] = vector[i]+1;
				}

			This kernel function takes as argument the range of values over which it should iterate. 
			
	*/
	oneapi::tbb::parallel_for( tbb::blocked_range<int>(0,10),
							   [&](tbb::blocked_range<int> r)
	{
		for (auto i = r.begin(); i != r.end(); ++i)
		{
			output[i] = vector[i]+1;
		}
	});

	for (int i=0; i<10; ++i){
		cout<< "vector[" << i << "] = " << vector [i] << '\t';
		cout<< "output[" << i << "] = " << output [i] << '\n';
	}
	return 0;
}
