#include <iostream>
#include <cstdlib>
#include "mpi.h"

int main(int argc, char **argv)
{
  MPI_Init(&argc, &argv);
  int pid = 0, np = 0;

  MPI_Comm_rank(MPI_COMM_WORLD, &pid);
  MPI_Comm_size(MPI_COMM_WORLD, &nproc);

  int N = 0;
  int tag = 0;
  int src = (pid-1+np)%np;
  int dest = (pid+1)%np;
  
  if (0 == pid) {
      MPI_Send(&N, 1, MPI_INT, dest, tag, MPI_COMM_WORLD);
      MPI_Recv(&N, 1, MPI_INT, src, tag, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    } else {
      MPI_Recv(&N, 1, MPI_INT, src, tag, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
      N++;
      MPI_Send(&N, 1, MPI_INT, dest, tag, MPI_COMM_WORLD);
    }

     if (0 == pid) {
       std::cout << N << std::endl;
     }
     
  
  MPI_Finalize();
  return 0;
}
