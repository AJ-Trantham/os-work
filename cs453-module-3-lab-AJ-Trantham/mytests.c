#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/times.h> /* for times system call */
#include <sys/time.h>  /* for gettimeofday system call */
#include <unistd.h>
#include <error.h>
#include "lab.h"

// function prototypes
int check_if_sorted(int A[], int n);
void generate_random_array(int A[], int n, int seed);
void serial_mergesort(int A[], int p, int r);
void threaded_mergesort(int A[], int p, int r, int thread_num);
void merge(int A[], int p, int q, int r);
void insertion_sort(int A[], int p, int r);
double getMilliSeconds(void);
void copy_array(int A[], int B[], int n);


/*
---------------------------------------------------------------------------
clock_t times(struct tms *buffer);
times() fills the structure pointed to by buffer with
time-accounting information.  The structure defined in
<sys/times.h> is as follows:
struct tms {
    clock_t tms_utime;       user time
    clock_t tms_stime;       system time
    clock_t tms_cutime;      user time, children
    clock_t tms_cstime;      system time, children
The time is given in units of 1/CLK_TCK seconds where the
value of CLK_TCK can be determined using the sysconf() function
with the agrgument _SC_CLK_TCK.
---------------------------------------------------------------------------
*/


float report_cpu_time(void)
{
	struct tms buffer;
	float cputime;

	times(&buffer);
	cputime = (buffer.tms_utime)/ (float) sysconf(_SC_CLK_TCK);
	return (cputime);
}


float report_sys_time(void)
{
	struct tms buffer;
	float systime;

	times(&buffer);
	systime = (buffer.tms_stime)/ (float) sysconf(_SC_CLK_TCK);
	return (systime);
}

double getMilliSeconds(void)
{
    struct timeval now;
    gettimeofday(&now, (struct timezone *)0);
    return (double) now.tv_sec*1000.0 + now.tv_usec/1000.0;
}


/*
 * generate_random_array(int A[], int n, int seed):
 *
 * description: Generate random integers in the range [0,RANGE]
 *              and store in A[1..n]
 */

#define RANGE 1000000

void generate_random_array(int A[], int n, int seed)
{
    int i;

	srandom(seed);
    for (i=1; i<=n; i++)
        A[i] = random()%RANGE;
}

void copy_array(int A[], int B[], int n) {
    int i;
    for (i=0; i <= n; i++ ) {
        B[i] = A[i];
    }
}


/*
 * check_if_sorted(int A[], int n):
 *
 * description: returns TRUE if A[1..n] are sorted in nondecreasing order
 *              otherwise returns FALSE
 */

int check_if_sorted(int A[], int n)
{
	int i=0;

	for (i=1; i<n; i++) {
		if (A[i] > A[i+1]) {
			return FALSE;
		}
	}
	return TRUE;
}

int test_effectivness(void){
    int thread_num = 5;
    int seed = 1;

    FILE *file = fopen("effectivness-test.csv", "w");
    if (file == NULL) return -1;
    fprintf(file, "Elements, Serial Time, Parallel Time\n");

    int i;
    for(i = 1; i <= 100; i++) {
        int n = i * 1000000;
        int *S = (int *) malloc(sizeof(int) * (n+1)); // n+1 since we are using A[1]..A[n]
        int *P = (int *) malloc(sizeof(int) * (n+1));

        // generate random input
        generate_random_array(S, n, seed);
        generate_random_array(P, n, seed);

        double start_time_S;
        double sorting_time_S;
        double start_time_P;
        double sorting_time_P;

        // sort the input (and time it) with serial approach
        start_time_S = getMilliSeconds();
        serial_mergesort(S,1,n);
        sorting_time_S = (getMilliSeconds() - start_time_S)/1000.0;

        // sort input (and time it) with multi-threaded approach
        start_time_P = getMilliSeconds();
        threaded_mergesort(P, 1, n, thread_num);
        sorting_time_P = (getMilliSeconds() - start_time_P)/1000.0;

        if (check_if_sorted(S,n)) {
            printf("Sorting %d elements took %4.2lf seconds with Serial Algorithm.\n", n,  sorting_time_S);
        } else {
            printf("sorting S failed!!!!\n");
            exit(EXIT_FAILURE);
        }

        // print results if correctly sorted otherwise cry foul and exit
        if (check_if_sorted(P,n)) {
            printf("Sorting %d elements took %4.2lf seconds with multi-threaded Algorithm.\n", n,  sorting_time_P);
        } else {
            exit(EXIT_FAILURE);
        }

        fprintf(file,"%d, %f, %f\n", n, sorting_time_S, sorting_time_P);

        free(S);
        free(P);
    }
    fclose(file);
    return 0;
}

int test_efficency(void){
    int n = 100000000;
    int seed = 1;

    FILE *file = fopen("efficency-test.csv", "w");
    if (file == NULL) return -1;
    fprintf(file, "Serial Time, Thread Number, Parallel Time\n");

    int i;
    for(i = 2; i < 9; i++) {
        int *S = (int *) malloc(sizeof(int) * (n+1)); // n+1 since we are using A[1]..A[n]
        int *P = (int *) malloc(sizeof(int) * (n+1));
        // generate random input
        generate_random_array(S, n, seed);
        generate_random_array(P, n, seed);

        double start_time_S;
        double sorting_time_S;
        double start_time_P;
        double sorting_time_P;

        // sort the input (and time it) with serial approach
        start_time_S = getMilliSeconds();
        serial_mergesort(S,1,n);
        sorting_time_S = (getMilliSeconds() - start_time_S)/1000.0;

        // sort input (and time it) with multi-threaded approach
        start_time_P = getMilliSeconds();
        threaded_mergesort(P, 1, n, i);
        sorting_time_P = (getMilliSeconds() - start_time_P)/1000.0;

        if (check_if_sorted(S,n)) {
            printf("Sorting %d elements took %4.2lf seconds with Serial Algorithm.\n", n,  sorting_time_S);
        } else {
            printf("sorting S failed!!!!\n");
            exit(EXIT_FAILURE);
        }

        // print results if correctly sorted otherwise cry foul and exit
        if (check_if_sorted(P,n)) {
            printf("Sorting %d elements took %4.2lf seconds with multi-threaded Algorithm.\n", n,  sorting_time_P);
        } else {
            exit(EXIT_FAILURE);
        }

        fprintf(file,"%f, %d, %f\n", sorting_time_S, i, sorting_time_P);

        free(S);
        free(P);
    }
    fclose(file);
    return 0;
}

void print_array(int A[],int n) {
    int i;
    printf("Array: ");
    for (i=1; i<=n; i++) {
        printf(", %d",A[i]);
    }
    printf("\n");
}

int main(int argc, char **argv) {

	if (argc < 3) { // there must be at least two command-line argument
			fprintf(stderr, "Usage: %s <input size>  <number of threads> [<seed>] \n", argv[0]);
			exit(1);
	}

	int n = atoi(argv[1]);
	int thread_number = atoi(argv[2]);
	int seed = 1;
	if (argc == 4)
		seed = atoi(argv[3]);

	int *A = (int *) malloc(sizeof(int) * (n+1)); // n+1 since we are using A[1]..A[n]
	int *B = (int *) malloc(sizeof(int) * (n+1)); // matches A

	// generate random input
	generate_random_array(A,n, seed);
	generate_random_array(B, n, seed);

	double start_time_A;
	double sorting_time_A;
	double start_time_B;
	double sorting_time_B;

	// sort the input (and time it) with serial approach
	start_time_A = getMilliSeconds();
	serial_mergesort(A,1,n);
	sorting_time_A = getMilliSeconds() - start_time_A;

	// sort input (and time it) with multi-threaded approach
	start_time_B = getMilliSeconds();
    threaded_mergesort(B, 1, n, thread_number);
    sorting_time_B = getMilliSeconds() - start_time_B;

	// print results if correctly sorted otherwise cry foul and exit
	if (check_if_sorted(A,n)) {
		printf("Sorting %d elements took %4.2lf seconds with Serial Algorithm.\n", n,  sorting_time_A/1000.0);
//		printf("Array A sorted: ");
//		print_array(A,n);
	} else {
		printf("%s: sorting A failed!!!!\n", argv[0]);
		exit(EXIT_FAILURE);
	}

    // print results if correctly sorted otherwise cry foul and exit
    if (check_if_sorted(B,n)) {
        printf("Sorting %d elements took %4.2lf seconds with multi-threaded Algorithm.\n", n,  sorting_time_B/1000.0);
    } else {
//        printf("%s: sorting B failed!!!!\n", argv[0]);
//        printf("Array B sorting fail ");
        print_array(B,n);
        exit(EXIT_FAILURE);
    }

	free(A);
    free(B);

    //test_efficency();
    //test_effectivness();

	exit(EXIT_SUCCESS);
}
