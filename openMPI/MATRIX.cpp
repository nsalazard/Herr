#include <iostream>
#include <cstdlib>
#include <cmath>
#include "mpi.h"

void fill(int nsize, int pid, int np);

int main(int argc, char **argv)
{
  MPI_Init(&argc, &argv); /* Mandatory */

  int pid;                 /* rank of process */
  int np;                 /* number of processes */

  MPI_Comm_size(MPI_COMM_WORLD, &np);
  MPI_Comm_rank(MPI_COMM_WORLD, &pid);

  int NSIZE = std::atoi(argv[1]);
  double start = 0; 

  if (0 == pid) {
    std::cout << "Comm time: " << end-start << "\n";
  }
  
  fill(NSIZE, pid, np);

  MPI_Finalize(); /* Mandatory */
  
  return 0; 
}

void fill(int nsize, int pid, int np)
{
  int tag1 = 0, tag2 = 1;

  int Nlocal = nsize/np;
  int Ntotal = Nlocal * nsize;
  double *data = new double [Ntotal] {0};

  // falta inicializar el arreglo!
  for (int ii = 0; ii < Ntotal; ++ii) {
    data[ii] = 0;
  }

  for (int ii = Nlocal*pid; ii < Ntotal; ii += nsize+1) {
    data[ii] = 1;
  }
  
 print2(data,nsize,nlocal,pid,np);
  
  delete [] data;
}

void print1(const data_t & data, int nx, int ny)
{
    for(int ix = 0; ix < nx; ++ix) {
        for(int iy = 0; iy < ny; ++iy) {
            std::cout << data[ix*nx + iy] << "  ";
        }
        std::cout << "\n";
    }
    std::cout << "\n";
}

void print2(const data_t & data, int nx, int ny, int pid, int np)
{
    int tag = 0;
    if (0 == pid) {
        print1(data, nx, ny);
        std::vector<double> dat(nx*ny);
        for (int src = 1; src < np; ++src) {
            MPI_Recv(&dat[0], nx*ny, MPI_DOUBLE, src, tag, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            print_screen(dat, nx, ny);
        }
    } else {
        int dest = 0;
        MPI_Send(&data[0], nx*ny, MPI_DOUBLE, dest, tag, MPI_COMM_WORLD);
    }
}
