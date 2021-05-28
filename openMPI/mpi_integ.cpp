#include <cstdio>
#include "mpi.h"

double f (double x){
	return x*x;
}

//Parameters
const int numberRects = 5000;
const double xmin = 2.0;
const double xmax = 5.0;

int main(int argc, char **argv)
{
	int ii;
	double area,at,height, width;

	int pid,np;
  MPI_Init(&argc, &argv); /* Mandatory */

	MPI_Comm_rank(MPI_COMM_WORLD, &pid);
	MPI_Comm_size(MPI_COMM_WORLD, &np);

	int nrect =numberRects/np;
	int imin = pid * nrect;

// MPI TIMER
	double tstart = MPI_Wtime();
	area = 0.0;
	width = (xmax -xmin)/numberRects;

	for(ii= imin; ii < imin + nrect; ++ii){
		at = xmin + ii *width + width/2.0;
		height = f(at);
		area = area + width * height;
	}
double tend = MPI_Wtime();
std::printf("Time from pid %d: %lf \n", pid, tend-tstart);
	std::printf("The area from %lf to %lf is: %le \n", xmin + (imin*width),xmin + (imin*nrect)*width, area);

	int tag = 0;
	if(0 ==pid){
		double total = area;
		for(int src = 1; src < np; ++src){
		MPI_Recv(&area,1,MPI_DOUBLE,src,tag,MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		total += area;}
		std::printf ("El area total es %f \n", total);
	}
	else{
		int dest = 0;
		MPI_Send(&area,1,MPI_DOUBLE,dest,tag,MPI_COMM_WORLD);

	}



  MPI_Finalize(); /* Mandatory */
  return 0;
}
