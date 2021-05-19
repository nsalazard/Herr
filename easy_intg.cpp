#include <iostream>
#include <cmath>
#include <omp.h>


const double XMIN = 0.0;
const double XMAX = 10.0;
const int N = 1000000;
const double DX = (XMAX-XMIN)/N;


double fun (double x);

int main(void){

double suma =0.0;
#pragma omp parallel for reduction(+:suma)
for(int ii =0; ii < N; ++ii){
    double x = XMIN + ii*DX;
    suma += fun(x);
}

suma *= DX;
std::cout << suma << "\n";
}

double fun(double x){

    return x*x;
}
