#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

void insertionSort(double[] arr);  

void main(int argc, char *argv[])
{
    if (argc != 2)
    {
        fprintf(stderr, "%s", "usage: prog <Number of threads>\n");
        exit(1);
    }
    //n will be given as command line argument
    srand(time(NULL));
    int nThreads = atoi(argv[1]);
    struct timespec ts_begin, ts_end;
    double elapsed;
    int i;
    pthread_t thread;

    //create array A(n double values) and randomly generate these values
    double* arrayA = malloc(sizeof(*arrayA) * nThreads);
    for (i = 0; i < nThreads; i++) {
        double randNum = ((double)rand() / (double)1000) + 1;
        arrayA[i] = randNum;
    }
    //also create arrays B and C with the same size of A
    double* arrayB = malloc(sizeof(*arrayB) * nThreads);
    double* arrayC = malloc(sizeof(*arrayC) * nThreads);
    //Afirsthalfand AsecondHalf with the half size of A
    int halfThreadSize = nThreads / 2;

    double *AfirstHalf = malloc(sizeof(*arrayA) * halfThreadSize);
    double *AsecondHalf = malloc(sizeof(*arrayA) * halfThreadSize);
    //-------------------- ONE THREAD CASE --------------------
    //copy A into B
    for (i = 0; i < nThreads; i++) {
        arrayB[i] = arrayA[i];
    }
    if (clock_gettime(CLOCK_MONOTONIC, &ts_begin)) {
        fprintf(stderr, "%s", "Failed to get the start time\n");
        exit(1);
    }
    
    //create thB sortThread to sort B  /* use selection/insertion sort O(n^2) */
    pthread_create(&thread, 0, insertionSort, arrayB);
        //join thB
    
    if (clock_gettime(CLOCK_MONOTONIC, &ts_end)) {
        fprintf(stderr, "%s", "Failed to get the end time\n");
        exit(1);
    }
    elapsed = ts_end.tv_sec - ts_begin.tv_sec;
    elapsed += (ts_end.tv_nsec - ts_begin.tv_nsec) / 1000000000.0;

    //Sorting by ONE thread is done in elapsed * 1000 ms

        //-------------------- TWO THREADS CASE --------------------
        //copy A into Afirsthalf and AsecondHalf
    if (clock_gettime(CLOCK_MONOTONIC, &ts_begin)) {
        fprintf(stderr, "%s", "Failed to get the start time\n");
        exit(1);
    }
    //create thA1  sortThread to sort Afirsthalf   /* use selection/insertion sort O((n/2)^2) */
        //create thA2  sortThread to sort Asecondhalf    /* use selection/insertion sort O((n/2)^2) */

        //join thA1
        //join thA2

        //create thM  mergeThread to merge Afirsthalfand Asecondhalf into C
        /* make sure this just merges the sorted values form two arrays while keeping them sorted.  O(n) */
        /* dont' copy these arrays and then call sort! which will be like sorting the whole thing! */
        //join thM
    if (clock_gettime(CLOCK_MONOTONIC, &ts_end)) {
        fprintf(stderr, "%s", "Failed to get the end time\n");
        exit(1);
    }
    elapsed = ts_end.tv_sec - ts_begin.tv_sec;
    elapsed += (ts_end.tv_nsec - ts_begin.tv_nsec) / 1000000000.0;

    //Sorting by TWO threads is done in elapsed * 1000 ms
}

void insertionSort(double[] arr) {

}