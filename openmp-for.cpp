#include <iostream>
#include <omp.h>
#include <cmath>


int main(void){

	const int N = 10000000;
	int * data = nullptr;
	data = new int [N];


#pragma omp parallel for
	for(int ii =0; ii < N; ++ii){
		data[ii] = 2*std::sin(ii);
	}

	std::cout << data[N/2] << std::endl;
	delete [] data;
	return 0;
	
}
