#include<random>
#include<iostream>
#include <vector>
#include "mpi.h"
#include <string>
#include <fstream>
double samples(int N, int processId);
int main (int argc, char **argv)
{
  std::cout.precision(7); std::cout.setf(std::ios::scientific);
  /* MPI variables*/
  int dest, np, processId, src, tag;
  MPI_Status status;
  /* Problem variables*/
  int N=std::atoi(argv[1]);
  int P=std::atof(argv[2]);
  double Nc;
  int range;
  double sum;
  double t1,t2;
  double T;
  double Ttotal;
  /* MPI setup */
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &np);
    MPI_Comm_rank(MPI_COMM_WORLD, &processId);
  /* Broadcast the read data parameters*/
   MPI_Bcast(&N,1, MPI_INT,0, MPI_COMM_WORLD);
   MPI_Barrier(MPI_COMM_WORLD);
    t1=MPI_Wtime();
  /* Adjust problem size for sub-process */
    range=N/np;
  /* Calculate sum for subproblem */
    Nc=samples(range,processId);
 
  /* Collect and reduce data */
    MPI_Reduce(&Nc, &sum, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
    MPI_Barrier(MPI_COMM_WORLD);  
    t2=MPI_Wtime();
    T=t2-t1;
  /*  std::cout<<"T "<<T<<" processId "<<processId<<std::endl; To check the time spent for different processes */
    MPI_Reduce(&T, &Ttotal, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);
    if(0==processId && P!=-1.0){ /* Master */
      sum=4*sum/N;
      std::cout<<sum<<"\t";
  /*  std::cout<<(P)/Ttotal<<"\t"<<P/(Ttotal*np)<<"\t"<<np<<std::endl; To get the metrics for a given value P */ 
    }
    else{
      if(0==processId)
	{
      std::cout<<Ttotal<<std::endl;
        }
	}
    /* finish */

   MPI_Finalize();
    
  
  
}

double samples(int N,int processId)
{
    int count = 0;
    int seed = processId; // use pid for seed when in parallel
    std::mt19937 gen(seed);
    std::uniform_real_distribution<double> dis(-1.0, 1.0);
    for (int ii = 0; ii < N; ++ii) {
    double rx = dis(gen);
    double ry = dis(gen);
    if (rx*rx + ry*ry < 1)
      count++;
  }
  return count;
}

