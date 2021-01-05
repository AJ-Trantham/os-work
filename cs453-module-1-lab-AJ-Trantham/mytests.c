#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lab.h"

enum functions { MYSTRNCPY,MYSTRNCAT, MYSTRNCMP };
enum results {PASS = 0, FAIL = -1};

// function prototypes
void printPass(void);
void printFail(char * expected, char * actual);
int testFunction(enum functions func, char *s1, char *s2, size_t n);


/** Changes text Color to green and prints pass. Helper for testFunction */
void printPass(void) {
    printf("\033[0;32m");
    printf("PASS\n");
    printf("\033[0m") ;
}

/** Prints Fail message and results for functions that return char * */
void printFail(char * expected, char * actual) {
    printf("\033[0;31m"); //Set the text to the color red
    printf("FAIL\n");
    printf("Expected Result: %s\n", expected);
    printf("Actual Result: %s\n", actual);
    printf("\033[0m"); //Resets the text to default color
}

int testFunction(enum functions func, char *s1, char *s2, size_t n) {
    if (func == MYSTRNCMP) {
        printf("Test: mystrncmp(%s, %s, %lu) \n", s1, s2, n);
        if (strncmp(s1, s2, n) == mystrncmp(s1, s2, n) || (strncmp(s1, s2, n) < 0 && mystrncmp(s1, s2, n) < 0) || (strncmp(s1, s2, n) > 0 && mystrncmp(s1, s2, n) > 0)) {
            printPass();
            return PASS;
        } else {
            printf("FAIL\n");
            printf("Expected Result: %d\n", strncmp(s1, s2, n));
            printf("Actual Result: %d\n", mystrncmp(s1, s2, n));
            return FAIL;
        }
    } else if (func == MYSTRNCAT) {
        printf("Test: mystrncat(%s, %s, %lu) \n", s1, s2, n);
        // check the null case
        if (!s1 || !s2) {
            char * myResult = mystrncat(s1, s2, n);
            // If either argument is null, mystrncat should return null
            if (!myResult) {
                printPass();
                return PASS;
            } else {
                printf("\033[0;31m"); //Set the text to the color red
                printf("FAIL\n");
                printf("\033[0m"); //Resets the text to default color
                return FAIL;
            }
        } else { //s1 must be set here, so strLen is safe as long as string is nul terminated - assumption
            int s1Len = strlen(s1);
            char * s1Cpy = calloc(s1Len + n + 1, sizeof(char)); // calloc will initialize the memory to 0
            s1Cpy = strncpy(s1Cpy, s1, s1Len);
            char * myResult = mystrncat(s1, s2, n);
            char * compResult = strncat(s1Cpy, s2, n);
            int catLen = s1Len + n +1;
            if (strncmp(compResult, myResult, catLen) == 0) {
                printPass();
                free(s1Cpy);
                return PASS;
            } else {
                printFail(compResult, myResult);
                free(s1Cpy);
                return FAIL;
            }
        }
    } else if (func == MYSTRNCPY) {
        printf("Test: mystrncpy(%s, %s, %lu) \n", s1, s2, n);
        if (!s1 || !s2) { // check the null case
            char *myResult = mystrncpy(s1, s2, n);
            // If either argument is null, mystrncat should return null
            if ((!s1 || !s2) && !myResult) {
                printPass();
                return PASS;
            } else {
                printf("\033[0;31m"); //Set the text to the color red
                printf("FAIL\n");
                printf("\033[0m"); //Resets the text to default color
                return FAIL;
            }
        } else {
            //int s1Len = strlen(s2);
            char * s1Cpy = malloc(n+1);
            s1Cpy = strncpy(s1Cpy, s1, n+1);
            char * myResult = mystrncpy(s1, s2, n+1);
            char * compResult = strncpy(s1Cpy, s2, n+1);
            if (strncmp(myResult, compResult, n) == 0) {
                printPass();
                free(s1Cpy);
                return PASS;
            } else {
                printFail(compResult, myResult);
                free(s1Cpy);
                return FAIL;
            }
        }
    } else {
        printf(" Error, Function not recognized!");
        return FAIL;
    }
}

int main(void)
{
    /* Test mystrncpy */
    printf("Running tests for mystrncpy\n");
    testFunction(MYSTRNCPY, NULL, NULL, 0);
    char dest1[32] = "";
    char src1[32] = "string1";
    char dest2[32] = "";
    char src2[32] = "string2";
    char dest3[32] = "";
    char src3[32] = "string3";
    testFunction(MYSTRNCPY, dest1, src1, 7);
    testFunction(MYSTRNCPY, dest2, src2, 3);
    testFunction(MYSTRNCPY, dest3, src3, 10);
    printf("\n");

    /* Test mystrncmp */
    printf("Running tests for mystrncmp\n");
    testFunction(MYSTRNCMP, NULL,NULL, 0);
    testFunction(MYSTRNCMP, "string", "string",5);
    testFunction(MYSTRNCMP, "string", "string",1);
    testFunction(MYSTRNCMP, "string", "string",10);
    testFunction(MYSTRNCMP, "a", "b",1);
    testFunction(MYSTRNCMP, "aaa", "aad", 3);
    testFunction(MYSTRNCMP, "aaa", "aab", 2);
    printf("\n");

    /* Test mystrncat */
    printf("Running tests for mystrncat\n");
    char * s1 = NULL;
    char * s2 = NULL;
    char s3[32] = "string";
    char s4[32] = "cat";
    char s5[32] = "fender";
    char s6[32] = "guitar";
    char s7[32] = "fender";
    char s8[32] = "guitar";
    testFunction(MYSTRNCAT, s1, s2, 0);
    testFunction(MYSTRNCAT, s3, s2, 2);
    testFunction(MYSTRNCAT, s3, s4, 3);
    testFunction(MYSTRNCAT, s5, s6, 6);
    testFunction(MYSTRNCAT, s7, s8, 3);
    printf("\n");

     return 0;
}
