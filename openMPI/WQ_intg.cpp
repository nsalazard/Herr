#include <iostream>
#include <cstdlib>
#include <cmath>
#include "mpi.h"

using fptr = double(double);

double f(double x);
double integral(double a, double b, int nsteps, fptr fun, int pid, int np);
void sum_results(double val, int pid, int np);

int main(int argc, char **argv)
{
  MPI_Init(&argc, &argv); /* Mandatory */

  int pid;                 /* rank of process */
  int np;                 /* number of processes */

  MPI_Comm_size(MPI_COMM_WORLD, &np);
  MPI_Comm_rank(MPI_COMM_WORLD, &pid);

  double a = std::atof(argv[1]);
  double b = std::atof(argv[2]);
  int N = std::atoi(argv[3]);;

  double t0 = MPI_Wtime();
  double val = integral(a, b, N, f, pid, np);
  double t1 = MPI_Wtime();
  std::cout << "TIME: " << t1-t0 << std::endl;
  
  sum_results(val, pid, np);
  
  MPI_Finalize(); /* Mandatory */
  
  return 0;
}

double f(double x)
{
  return x*std::sin(x)/(1 + std::sqrt(1+x));
}

double integral(double a, double b, int nsteps, fptr fun, int pid, int np)
{
  double result = 0;
  int Nlocal = nsteps/np;
  double dx = (b-a)/nsteps;
  int iimin = pid*Nlocal;
  int iimax = (pid+1)*Nlocal;
  for (int ii = iimin; ii < iimax; ++ii) {
    double x = a + ii*dx;
    result += fun(x);
  }
  result *= dx;
  return result;
}

void sum_results(double val, int pid, int np)
{
  int tag = 0;
  if (pid != 0) {
    // enviar val a pid 0
    int dest = 0;
    MPI_Send(&val, 1, MPI_DOUBLE, dest, tag, MPI_COMM_WORLD);
  } else { // pid ==0
    // recibir de pid 1, 2, 3, 4, 5, ... , np-1
    double total = val;
    for (int ipid = 1; ipid < np; ++ipid) {
      MPI_Recv(&val, 1, MPI_DOUBLE, ipid, tag, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
      total += val;
    }
    // imprimir
    std::cout << "Total value: " << total << std::endl;
  }
}
