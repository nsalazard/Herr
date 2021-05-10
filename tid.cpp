#include <omp.h>
#include <cstdio.h>

int main(int argc, char *argv[]) {

    int nthreads, tid;

    /* Fork a team of threads with each thread having a private tid variable */
#pragma omp parallel private(tid)
    {

    /* Obtain and print thread id */
    tid = omp_get_thread_num();
    std::printf("Hello World from thread = %d\n", tid);

    /* Only master thread does this */
    if (tid == 0) 
{
    nthreads = omp_get_num_threads();
    std::printf("Number of threads = %d\n", nthreads);
}

    }  /* All threads join master thread and terminate */
     nthreads = omp_get_num_threads();
    std::printf("Number of threads = %d\n", nthreads);

}