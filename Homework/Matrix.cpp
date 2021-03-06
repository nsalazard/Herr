#include <iostream>
#include <cstdlib>
#include <cmath>
#include <vector>
#include "mpi.h"

typedef std::vector<double> VEC;

void fill(int nsize, int pid, int np);
void print1(const VEC & data, int nx, int ny);
void print2(const VEC & data, int nx, int ny, int pid, int np);

int main(int argc, char **argv)
{
  int NSIZE = std::atoi(argv[1]); 
  MPI_Init(&argc, &argv); /* Mandatory */

  int pid;                 /* rank of process */
  int np;                 /* number of processes */

  MPI_Comm_size(MPI_COMM_WORLD, &np);
  MPI_Comm_rank(MPI_COMM_WORLD, &pid);
  
  fill(NSIZE, pid, np);

  MPI_Finalize(); /* Mandatory */
  
  return 0; 
}

void fill(int nsize, int pid, int np)
{
  int Nlocal = nsize/np;
  int Ntotal = Nlocal * nsize;
  VEC data(Ntotal);

  // falta inicializar el arreglo!
  for (int ii = 0; ii < Ntotal; ++ii) {
    data[ii] = 0;
  }

  for (int ii = Nlocal*pid; ii < Ntotal; ii += nsize+1) {   //Danger
    data[ii] = 1.0;
  }
  

   print2(data,Nlocal, nsize,pid,np);
  

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

void print2(const VEC & data, int nx, int ny, int pid, int np)
{
    int tag = 0;
    if (0 == pid) {
        print1(data, nx, ny);
        VEC dt(nx*ny); 
        for (int src = 1; src < np; ++src) {
            MPI_Recv(&dt[0], nx*ny, MPI_DOUBLE, src, tag, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            print1(dt, nx, ny);
        }
    } else {
        int dest = 0;
        MPI_Send(&data[0], nx*ny, MPI_DOUBLE, dest, tag, MPI_COMM_WORLD);
    }   
}
