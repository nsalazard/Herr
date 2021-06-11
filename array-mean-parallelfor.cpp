#include <iostream>
#include <cstdlib>
#include <cmath>
#include <chrono>
#include <omp.h>

template <typename t1, typename t2>
void print_elapsed(t1 start, t2 end );
void fill(double * data, int n, int nthreads);
double average_mp(double * d, int n, int nthreads);

int main(int argc, char **argv)
{
  int N = std::atoi(argv[1]);
  int NTH = std::atoi(argv[2]);
  double * data = new double [N]; 

  fill(data, N, NTH);

  //average
  auto start = std::chrono::steady_clock::now();
  double avg = average_mp(data, N, NTH);
  auto end = std::chrono::steady_clock::now();
  std::cout << "Time-OpenMP: \t";
  print_elapsed(start, end);

  std::cout << "Average: " << avg << std::endl;
 
  delete [] data;
  return 0;
}

void fill(double * d, int n, int nthreads)
{
#pragma omp parallel for num_threads (nthreads)
  for (int ii = 0; ii < n; ++ii) {
      d[ii] = 2*std::sin(ii) + std::log(ii + 1);
    }
}

double average_mp(double * data, int n, int nthreads)
{
  double sum = 0.0;
#pragma omp parallel for num_threads (nthreads) reduction(+:sum)
  for (int ii = 0; ii < n; ++ii) {
    sum += data[ii]; 
  }
  return sum/n;
}

template <typename t1, typename t2>
void print_elapsed(t1 start, t2 end )
{
  std::cout.precision(6); std::cout.setf(std::ios::scientific);
  std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(end-start).count()/1000.0 << "\n";
}
