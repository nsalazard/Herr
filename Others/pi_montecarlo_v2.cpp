#include <iostream>
#include <cmath>
#include <cstdlib>
#include <random>
#include "mpi.h"
#include <chrono>

void parallel(int N, int argc, char**argv); //the parallelized code
double random_parallel (int pid, int np, int N);
void pi_results(double val, int pid, int np, int n); //sums the counting of the Nc counted for each process
//void pi(int count, int N); //prints an aproximated value of pi

int main(int argc, char**argv)
{

  std::cout.precision(10); std::cout.setf(std::ios::scientific);
  int N = std::atoi(argv[1]);
  
  parallel(N, argc, argv);
  
   return 0;
}

void parallel(int N, int argc, char**argv) //the parallelized code
{
  
  MPI_Init(&argc, &argv); //initialization of the processes in parallel

  double tbeg = MPI_Wtime();
  int pid; //variable which is going to be replaced by the number id of the current process
  int np; //variable which is going to be replaced by the total number of processes currently running
  
  MPI_Comm_size(MPI_COMM_WORLD, &np); //total number of processes
  MPI_Comm_rank(MPI_COMM_WORLD, &pid); //id number of the current process

  if(pid==0){
    std::cout << "\n #pi \t number of processes \t elapsed time \t speedup \t efficiency \n";
  }
  
  double val = random_parallel (pid, np, N);
  // MPI_Barrier (MPI_COMM_WORLD); //esta línea coloca una barrera a cada slot tal que hará que todos esperen hasta que todos los procesos finalicen, y así cuando se impriman, lo harán en orden (eso es lo que entiendo)
  pi_results(val, pid, np, N);
  
  // MPI_Barrier( MPI_COMM_WORLD );
  double elapsedTime = MPI_Wtime() - tbeg;
  double totalTime;
  //  MPI_Reduce( &elapsedTime, &totalTime, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD );
    if ( pid == 0 ) {
      double T1 = 464.26616810; //value of the elapsed time for one slot and N = 2000000000 only
      double speedup = T1/elapsedTime; //speedup of the parallel process
      double efficience = speedup/np; 
      std::cout << np << "\t" << elapsedTime << "\t" << speedup << "\t" << efficience << "\n" ;
    }
    
    MPI_Finalize(); //Ending of the parallel processes
}


double random_parallel(int pid, int np, int N)
{
int count = 0;
  int seed = pid; // use pid for seed when in parallel
  std::mt19937 gen(pid);
  std::uniform_real_distribution<double> dis(-1.0, 1.0);

  double localsize = (1.0*N)/np;
  int iimin = pid*localsize;
  int iimax = (pid+1)*localsize;
  
  for (int ii = iimin; ii < iimax; ++ii) {
    double rx = dis(gen);
    double ry = dis(gen);
    if (rx*rx + ry*ry < 1)
      count++;
  }
  return count;
}

void pi_results(double val, int pid, int np, int n)
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
    // imprimir valor de pi
    double p =(4.0*total)/n; //is important to do the multiplication by 4.0 first because nc and n are integers, therefore they can get truncated to cero because nc<n. 
    std::cout << p << "\t";
  }
}

/*
void pi (int nc, int n)
{
  double p =(4.0*nc)/n; //is important to do the multiplication by 4.0 first because nc and n are integers, therefore they can get truncated to cero because nc<n. 
  std::cout << "An aproximated value of pi is given by: " << p << std::endl;
  }*/


/*void parallel(int N) //LOS ARGUMENTOS DE ENTRADA DEBEN INCLUIR argc y argv, solo que no sé cómo incluirlos
{
  
  MPI_Init(&argc, &argv); //initialization of the processes in parallel
  
  int pid; //variable which is going to be replaced by the number id of the current process
  int np; //variable which is going to be replaced by the total number of processes currently running
  
  MPI_Comm_size(MPI_COMM_WORLD &np); //total number of processes
  MPI_Comm_rank(MPI_COMM_WORLD, &pid); //id number of the current process
  
  int count = 0;
  int seed = pid; // use pid for seed when in parallel
  std::mt19937 gen(seed);
  std::uniform_real_distribution<double> dis(-1.0, 1.0);

  double localsize = (1.0*N)/np
  
  for (int ii = 0; ii < N; ++ii) {
    double rx = dis(gen);
    double ry = dis(gen);
    if (rx*rx + ry*ry < 1)
      count++;
  }
  sum_results(count, pid, np);
  
  pi(count, N);
  
  MPI_Finalize(); //Ending of the parallel processes
  }*/
  




