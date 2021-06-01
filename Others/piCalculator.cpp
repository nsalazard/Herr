#include <iostream>
#include <cstdlib>
#include <cmath>
#include "mpi.h"
#include <random>


double  Calculator(int N, int pid, int np);
int  CountC(int N,int pid, double from, double to);
void sum_results(double val, int pid, int np);


int main(int argc, char **argv)
{
  int N=std::atoi(argv[1]);
  
  MPI_Init(&argc, &argv); /* Mandatory */

  int pid;                 /* rank of process */
  int np;                 /* number of processes */
  
  MPI_Comm_size(MPI_COMM_WORLD, &np);
  MPI_Comm_rank(MPI_COMM_WORLD, &pid);

  double t0 = MPI_Wtime();
  double Partial_pi=Calculator (N,pid, np);
  double t1 = MPI_Wtime();
  std::cout << "Time: " << t1-t0 << std::endl;
  
  sum_results(Partial_pi, pid, np);
  
  MPI_Finalize(); /* Mandatory */
  
  return 0;
}

double Calculator(int N, int pid, int np)
{
  int range = 2; //Distancia entre -1 y 1
  double Portion=range/np;
  double from=-1+(Portion*pid);
  double to=-1+((pid+1)*Portion);
  double N_per_portion=N/np;
  double N_c= CountC(N_per_portion,pid,from, to);
  double result=0;
  result=4*N_c/N;
  return result;
  
}

int  CountC(int N,int pid, double from, double to)
{
    int count = 0;
    int seed = pid; // use pid for seed when in parallel
    std::mt19937 gen(seed);
    std::uniform_real_distribution<double> dis(from, to);
    for (int ii = 0; ii < N; ++ii) {
    double rx = dis(gen);
    double ry = dis(gen);
    if (rx*rx + ry*ry < 1)
      count++;
  }
 return count;  
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
    std::cout.precision(15); std::cout.setf(std::ios::scientific);
    std::cout << "Total value: " << total << std::endl;
  }
}
