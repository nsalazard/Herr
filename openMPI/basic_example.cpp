#include <iostream>
#include "mpi.h"

int main(int argc, char **argv)
{
  MPI_Init(&argc, &argv); /* Mandatory */


  MPI_Finalize(); /* Mandatory */
  return 0;
}