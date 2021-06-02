#include <iostream>
#include "mpi.h"

int main(int argc, char **argv)
{
  MPI_Init(&argc, &argv); /* Mandatory */

  int pid;                 /* rank of process */
  int np;                 /* number of processes */
  
  int N = std::atoi(argv[1]);

  MPI_Comm_size(MPI_COMM_WORLD, &np);
  MPI_Comm_rank(MPI_COMM_WORLD, &pid);
  
  double Nc = 0;
  int tag = 0;
  while (Nc < N){
  if (pid != 0) {
    // enviar Nc a pid 
    MPI_Send(&Nc, 1, MPI_INT, 0, tag, MPI_COMM_WORLD);
    MPI_Recv(&Nc, 1, MPI_INT, 0, tag, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
  } else { // pid ==0
    // recibir de pid 1, 2, 3, 4, 5, ... , np-1
      MPI_Recv(&Nc, 1, MPI_INT, 1, tag, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
      Nc += 1;
      MPI_Send(&Nc, 1, MPI_INT, 1, tag, MPI_COMM_WORLD);
      
    // imprimir
  }
  if (0 == pid) {
    std::cout << Nc << std::endl;
  }
  }
  
  MPI_Finalize(); /* Mandatory */
  
  return 0;
}
