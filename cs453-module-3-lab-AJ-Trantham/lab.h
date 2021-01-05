#ifndef __LAB_H
#define __LAB_H

#ifdef __cplusplus
extern "C" {
#endif

#define TRUE 1
#define FALSE 0
#include <pthread.h>

// function prototypes
void serial_mergesort(int A[], int p, int r);
void threaded_mergesort(int A[], int p, int r,  int thread_num);
void merge(int A[], int p, int q, int r);
void insertion_sort(int A[], int p, int r);
void locking_mergesort(int A[], int p, int r);

const int INSERTION_SORT_THRESHOLD = 100; //based on trial and error
const int SERIAL_SORT_THRESHOLD = 0; // TODO: need to figure out what is the correct number here
const int MAX_THREADS = 100000000;

#ifdef __cplusplus
}
#endif
#endif //__LAB_H
