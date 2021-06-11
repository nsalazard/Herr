#include <iostream>
#include <cstdlib>
#include <cmath>
#include <chrono>
#include <omp.h>
#include "mpi.h"

template <typename t1, typename t2>
void print_elapsed(t1 start, t2 end );
void fill(double * data, int n, int nthreads);
int average_mpi(double *data, int N, int pid, int np);
void comm(int N, int Nc, int pid, int np);

int main(int argc, char **argv)
{
  int N = std::atoi(argv[1]);
  double * data = new double [N]; 

  fill(data, N, NTH);
  
  MPI_Init(&argc, &argv); /* Mandatory */

  int pid;                 /* rank of process */
  int np;                 /* number of processes */
  MPI_Comm_size(MPI_COMM_WORLD, &np);
  MPI_Comm_rank(MPI_COMM_WORLD, &pid);
  double t0 = MPI_Wtime();
  int Nc = average_mpi(data, N, pid, np);
  comm(N, Nc, pid, np);
  double t1 = MPI_Wtime();
  std::cout << "TIME Open-MPI: " << t1-t0 << std::endl;
  
  MPI_Finalize();
 
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

void comm(int N, int Nc, int pid, int np)
{
  int tag = 0;
  if (pid != 0) {
    // enviar Nc a pid 0
    int dest = 0;
    MPI_Send(&Nc, 1, MPI_INT, dest, tag, MPI_COMM_WORLD);
  } else { // pid ==0
    // recibir de pid 1, 2, 3, 4, 5, ... , np-1
    double total = Nc;
    for (int ipid = 1; ipid < np; ++ipid) {
      MPI_Recv(&Nc, 1, MPI_INT, ipid, tag, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
      total += Nc;
    }
    // imprimir
    total = total/N; 
    std::cout << "Average OpenMPI: " << total << std::endl;
  }
}
int average_mpi(double *data, int N, int pid, int np)
{
  int num = N/np;  
  int imin = pid*num;
  int imax = (pid+1)*num;
  double total = 0;
    for (int ii = imin; ii < imax; ++ii) {
     total += data[ii];
  }
  return total;
}

template <typename t1, typename t2>
void print_elapsed(t1 start, t2 end )
{
  std::cout.precision(6); std::cout.setf(std::ios::scientific);
  std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(end-start).count()/1000.0 << "\n";
}
