#include <iostream>
#include <cstdlib>
#include <cmath>
#include <random>
#include "mpi.h"

double montecarlo( int nsteps, int pid, int np);
void sum_results(double val, int pid, int np);


int main(int argc, char **argv)
{
  MPI_Init(&argc, &argv); /* Mandatory */

  int pid;                 /* rank of process */
  int np;                 /* number of processes */

  MPI_Comm_size(MPI_COMM_WORLD, &np);
  MPI_Comm_rank(MPI_COMM_WORLD, &pid);
  int N = std::atoi(argv[1]);;
  double t0 = MPI_Wtime();
  double val = montecarlo(N, pid, np);
  double t1 = MPI_Wtime();
  std::cout << "TIME: " << t1-t0 << std::endl;

  sum_results(val, pid, np);

  MPI_Finalize(); /* Mandatory */

  return 0;
}



double montecarlo( int nsteps, int pid, int np)
{
  int Nlocal = nsteps/np;
  int iimin = pid*Nlocal;
  int iimax = (pid+1)*Nlocal;
  double pointCircles;
  double pointSquare = nsteps;
  double piaprox;
  int seed = pid; // use pid for seed when in parallel

  std::mt19937 gen(seed);
  std::uniform_real_distribution<double> dis(-1.0, 1.0);

  for (int ii = iimin; ii < iimax; ++ii) {
    double rx = dis(gen);
    double ry = dis(gen);
    double d = (rx*rx)+(ry*ry);
    if (d< 1){
        pointCircles++;
    }

  }
  piaprox = 4*(pointCircles/pointSquare);

  return piaprox;
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
