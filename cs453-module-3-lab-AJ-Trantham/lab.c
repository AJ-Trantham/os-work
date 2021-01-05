/**
 * Name: AJ Trantham
 * Email: ajtrantham@u.boisestate.edu
 * Github Username: AJ Trantham
 */
#include "lab.h"
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <pthread.h>

pthread_mutex_t mutexsum;
typedef struct thread_data tp;

struct thread_data {
    int thread_id;
    int *A;
    int p;
    int r;
};

/*
 * insertion_sort(int A[], int p, int r):
 *
 * description: Sort the section of the array A[p..r].
 */
void insertion_sort(int A[], int p, int r)
{
	int j;

	for (j=p+1; j<=r; j++) {
		int key = A[j];
		int i = j-1;
		while ((i > p-1) && (A[i] > key)) {
			A[i+1] = A[i];
			i--;
		}
		A[i+1] = key;
	}
}

/**
 * My threads will call this to perform sorting on
 * a segment of the list
 * @param thread_params
 * @return
 */
void *mergesort_for_thread(void *thread_params) {
    struct thread_data *my_data;
    my_data = (struct thread_data *) thread_params;
    locking_mergesort(my_data->A, my_data->p, my_data->r);
    pthread_exit((void*) thread_params);
}

/**
 * Sort the section of the array A[p..r].
 *
 * @param A
 * @param p
 * @param r
 * @param thread_num
 */
void threaded_mergesort(int A[], int p, int r, int thread_num) {

    if (thread_num > r || thread_num > MAX_THREADS) {
        thread_num = r;
    }

    // split the array into thread_num sections
    int split_len;
    if (thread_num == 1) {
        split_len = r; // basically don't split
    } else {
        split_len = r / thread_num;
    }

    int rem = (r % thread_num);

    // initialize thread_num worker threads - one for each of the above sections
    pthread_t thread[thread_num];
    pthread_attr_t attr;
    int rc;
    long t;
    void *status;

    // init thread and set detached attribute
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
    pthread_mutex_init(&mutexsum, NULL);
    struct thread_data thread_data_array[thread_num]; // used to pass the necessary params to the worker thread function
    int ending_index = split_len;
    // each thread will call basically do the serial_mergesort
    for (t = 0; t < thread_num; t++) {
        thread_data_array[t].thread_id = t;
        thread_data_array[t].A = A;
        thread_data_array[t].p = p;
        thread_data_array[t].r = ending_index;
        p += split_len;
        if (t == thread_num-2) {
            ending_index += split_len + rem; // on last iteration add the remaining indexes
        } else {
            ending_index += split_len;
        }
        rc = pthread_create(&thread[t], &attr, mergesort_for_thread, (void *) &thread_data_array[t]);
        if (rc) {
            printf("ERROR; received a return code from pthread_create() of %d\n", rc);
        }
    }

    // join threads up and exit
    pthread_attr_destroy(&attr);
    for(t=0; t<thread_num; t++) {
        rc = pthread_join(thread[t], &status);
        if (rc) {
            printf("ERROR; return code from pthread_join() is %d\n", rc);
            exit(-1);
        }
        //printf("Main: completed join with thread %ld having a statusof %ld\n",t,(long)status);
    }


        // I now have num_threads sorted lists and I need to merge them back together
        int i;
        int start = 1;
        int mid = split_len;
        int end = split_len *2;
        int merge_count = thread_num - 1; // number of list we need to add together
        for (i = 0; i < merge_count; i++) {
            merge(A, start, mid, end);
            mid = end;
            end += split_len;
            if (i == merge_count -2) {
                end += rem;
            }
        }
}


/*
 * serial_mergesort(int A[], int p, int r):
 *
 * description: Sort the section of the array A[p..r].
 */
void serial_mergesort(int A[], int p, int r)
{
	if (r-p+1 <= INSERTION_SORT_THRESHOLD)  {
			insertion_sort(A,p,r);
	} else {
		int q =  r + (p - r)/2;
		serial_mergesort(A, p, q);
		serial_mergesort(A, q+1, r);
		merge(A, p, q, r);
	}
}

void locking_mergesort(int A[], int p, int r)
{
    if (r-p+1 <= INSERTION_SORT_THRESHOLD)  {
        pthread_mutex_lock(&mutexsum);
        insertion_sort(A,p,r);
        pthread_mutex_unlock(&mutexsum);
    } else {
        int q =  r + (p - r)/2;
        serial_mergesort(A, p, q);
        serial_mergesort(A, q+1, r);
        pthread_mutex_lock(&mutexsum);
        merge(A, p, q, r);
        pthread_mutex_unlock(&mutexsum);
    }
}

/*
 * merge(int A[], int p, int q, int r):
 *
 * description: Merge two sorted sequences A[p..q] and A[q+1..r]
 *              and place merged output back in array A. Uses extra
 *              space proportional to A[p..r].
 */
void merge(int A[], int p, int q, int r)
{
	int *B = (int *) malloc(sizeof(int) * (r-p+1));

	int i = p;
	int j = q+1;
	int k = 0;
	int l;

	// as long as both lists have unexamined elements
	// this loop keeps executing.
	while ((i <= q) && (j <= r)) {
		if (A[i] < A[j]) {
			B[k] = A[i];
			i++;
		} else {
			B[k] = A[j];
			j++;
		}
		k++;
	}

	// now only at most one list has unprocessed elements.

	if (i <= q) {
		// copy remaining elements from the first list
		for (l=i; l<=q; l++) {
			B[k] = A[l];
			k++;
		}
	} else {
		// copy remaining elements from the second list
		for (l=j; l<=r; l++) {
			B[k] = A[l];
			k++;
		}
	}

	// copy merged output from array B back to array A
	k=0;
	for (l=p; l<=r; l++) {
		A[l] = B[k];
		k++;
	}

	free(B);
}
