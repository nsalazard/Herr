#include <iostream>
#include <cstdlib>
#include <cmath>
#include "mpi.h"
#include <random>
void pi(int N, int Nc, int pid, int np);
int samples(int N, int pid, int np);

int main(int argc, char **argv)
{
  std::cout.precision(15);
  std::cout.setf(std::ios::scientific);

  MPI_Init(&argc, &argv); /* Mandatory */

  int pid;                 /* rank of process */
  int np;                 /* number of processes */
  MPI_Comm_size(MPI_COMM_WORLD, &np);
  MPI_Comm_rank(MPI_COMM_WORLD, &pid);
  int N = std::atoi(argv[1]);;
  double t0 = MPI_Wtime();
  int Nc = samples(N, pid, np);
  double t1 = MPI_Wtime();
  std::cout << "TIME: " << t1-t0 << std::endl;
  MPI_Barrier (MPI_COMM_WORLD);
  pi(N, Nc, pid, np);
  
  MPI_Finalize(); /* Mandatory */
  
  return 0;
}
void pi(int N, int Nc, int pid, int np)
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
    total = 4*total/N; 
    std::cout << "El valor de Pi es: " << total << " usando " << np << " procesos " <<std::endl;
  }
}
int samples(int N, int pid, int np)
{
    int count = 0;
    // use pid for seed when in parallel
    std::mt19937 gen(pid);
    std::uniform_real_distribution<double> dis(-1.0, 1.0);
    N = N/np;
    for (int ii = 0; ii < N; ++ii) {
    double rx = dis(gen);
    double ry = dis(gen);
    if (rx*rx + ry*ry < 1)
      count++;
  }
  return count;
}
