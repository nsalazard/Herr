#include <iostream>
#include <cmath>
#include <omp.h>
#include <vector>
#include <numeric>

const double XMIN = 0.0;
const double XMAX = 10.0;
const int N = 1000000;
const double DX = (XMAX-XMIN)/N;

double fun (double x);

int main(void){
std::vector<double> sum;

#pragma omp parallel 
{
	int tid = omp_get_thread_num();
	int nthreads = omp_get_num_threads();

	if (tid == 0){
		sum.resize(nthreads);
	}
	# pragma omp barrier

	int size = N/nthreads;
	int imin = size *tid;
	int imax = imin + size;

	for(int ii = imin; ii < imax; ++ii){
			double x = XMIN + ii*DX;
			//No podemos sumar con una sola variable porque al tener tantos threads va a ver un race conditions
			sum[tid] += fun(x);
	}
	sum[tid]*= DX;
}
std::cout << std::accumulate(sum.begin(),sum.end(), 0.0) << "\n";
}

double fun(double x){

    return x*x;
}