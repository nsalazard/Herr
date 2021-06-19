#include <iostream>
#include <cstdlib>
#include <cmath>
#include <vector>
#include "mpi.h"

typedef std::vector<double> VEC;

void print1(const VEC & data, int nx, int ny);

int main(int argc, char **argv)
{
  int NSIZE = std::atoi(argv[1]); 
  MPI_Init(&argc, &argv); /* Mandatory */

  int pid;                 /* rank of process */
  int np;                 /* number of processes */

  MPI_Comm_size(MPI_COMM_WORLD, &np);
  MPI_Comm_rank(MPI_COMM_WORLD, &pid);
  

  int Nlocal = NSIZE/np;
  int Ntotal = Nlocal * NSIZE;
  VEC data(Ntotal);
  
  int nx = Nlocal;
  int ny = NSIZE;

  // falta inicializar el arreglo!
  for (int ii = 0; ii < Ntotal; ++ii) {
    data[ii] = 0;
  }

  for (int ii = 0; ii < Nlocal; ++ii) {  
    int iy = pid*Nlocal + ii;
    int ix = ii;
    data[ix*NSIZE + iy] = 1.0;
  }

  int tag = 0;
    if (0 == pid) {
        print1(data, nx, ny);
        //VEC copy(nx * ny);
        for (int src = 1; src < np; ++src) {
            MPI_Recv(&data[0], nx*ny, MPI_DOUBLE, src, tag, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            print1(data, nx, ny);
        }
    } else {
        int dest = 0;
        MPI_Send(&data[0], nx*ny, MPI_DOUBLE, dest, tag, MPI_COMM_WORLD);
    }   

  MPI_Finalize(); /* Mandatory */
  
  return 0; 
}

void print1(const VEC & data, int nx, int ny)
{
    for(int ix = 0; ix < nx; ++ix) {
        for(int iy = 0; iy < ny; ++iy) {
            std::cout << data[ix*ny + iy] << "  ";
        }
        std::cout << "\n";
    }
    std::cout << "\n";
}
