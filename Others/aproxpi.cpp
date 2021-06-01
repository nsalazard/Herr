#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <cmath>
#include <random>
#include <mpi.h>

int parasamples(int n, int pid, int np, double a, double b); 

int main (int argc, char **argv)
{
  MPI_Init(&argc, &argv);
  double t0= MPI_Wtime();//arranca el tiempo
  int pid;
  int np;

  MPI_Comm_size(MPI_COMM_WORLD, &np);
  MPI_Comm_rank(MPI_COMM_WORLD, &pid);

  double a= -1.0;
  double b= 1.0;
  int N= std::atoi(argv[1]);
  int tag= 0;

  int NC= parasamples(N, pid, np, a, b);
  
  if(pid!=0) {
    int dest= 0;
    
    MPI_Send(&NC, 1, MPI_INT, dest, tag, MPI_COMM_WORLD);
  } else {
    long int total= NC;
    
    for(int ipid=1; ipid< np; ++ipid)
      {
	MPI_Recv(&NC, 1, MPI_INT, ipid, tag, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
	total+= NC ;
	
      }
    double resultado= 4*total*(1.0/N);

    double t1= MPI_Wtime();// termina el tiempo
    double t= t1-t0;
    double speed= 266.559/t;// ese valor lo tome de una ejecución de referencia como T(2000000000,1)
    double parefi= speed/np;

    std::cout <<speed<< "\t" <<parefi<< "\n";
  }
  
  MPI_Finalize();
  return 0;
}


int parasamples(int n, int pid, int np, double a, double b)
{
  int count = 0;
  int res= n/np;
  double locn= (b-a)/np;
  double iimin= pid*locn+ a;
  double iimax= (pid+ 1)*locn + a;
  int seed = pid; // use pid for seed when in parallel
  std::mt19937 gen(seed);
  std::uniform_real_distribution<double> dis(-1.0, 1.0);

  for (int ii = 0; ii < res; ++ii) {
    double rx = dis(gen);
    double ry = dis(gen);
   
    if (rx*rx + ry*ry < 1) {
      count++;
    }
    
  } 
  return count;
}




int samples(int n)
{
    int count = 0;
    int seed = 1; // use pid for seed when in parallel
    std::mt19937 gen(seed);
    std::uniform_real_distribution<double> dis(-1.0, 1.0);
    for (int ii = 0; ii < n; ++ii) {
    double rx = dis(gen);
    double ry = dis(gen);
    if (rx*rx + ry*ry < 1)
      count++;
  }
  return count;
}
