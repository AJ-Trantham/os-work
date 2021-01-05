#include <stdio.h>
#include "lab.h"


void testP(void) {
    int argc = 2;
    char *a[2];
    a[0] = "mytests";
    a[1] = "-p";
    testmain(argc, a);
}

void testZ(void) {
    int argc = 2;
    char *z[2];
    z[0] = "mytests";
    z[1] = "-z";
    testmain(argc, z);
}

void testC(void) {
    int argc = 2;
    char *c[2];
    c[0] = "mytests";
    c[1] = "-c";
    testmain(argc, c);
}

void testD(void) {
    int argc= 2;
    char *c[4];
    c[0] = "mytests";
    c[1] = "-p";
    c[2] = "-d";
    c[3] = "/proc/";
    testmain(argc, c);
}

void testBad(void) {
    int argc= 2;
    char *c[2];
    c[0] = "mytests";
    c[1] = "-w";
    testmain(argc, c);
}

void testNoArgs(void) {
    int argc= 2;
    char *c[2];
    c[0] = "mytests";
    c[1] = "-t";
    testmain(argc, c);
}

int main(int argc, char **argv) {

    printf("Output for mytest - p: \n");
    testP();

    printf("\nOutput for mytest - c: \n");
    testC();

    printf("\nOutput for mytest - z: \n");
    testZ();

    printf("\nOutput for mytest - d: \n");
    testD();

    printf("Output for bad input: \n");
    testBad();

    printf("\nOutput for mytest : \n");
    testNoArgs();

    return 0;
}


