/**
 * Name: AJ Trantham
 * Email: ajtrantham@u.boisestate.edu
 * Github Username: aj-trantham
 */
#include "lab.h"
#include "string.h"
#include "stdlib.h"

char * mystrncpy(char *dest, const char *src, size_t n)
{
     /* If either dest or src are NULL return NULL */
     if(!dest || !src) return NULL;
     /* if n is 0 we are done! */
     if(0 == n) {
         dest = "";
         return dest;
     }

    size_t i = 0;
    while (i < n && src[i] != '\0') {
        dest[i] = src[i];
        i++;
    }
    // terminate str + fill extra space with null bytes
    while (i <= n) {
        dest[i] = '\0';
        i++;
    }

    return dest;
}

char * mystrncat(char *dest, const char *src, size_t n)
{
     /* If either dest or src are NULL return NULL */
     if(!dest || !src) return NULL;
     /* if n is 0 we are done! */
     if(0 == n) return dest;

     size_t i = 0;
     size_t destLen = strlen(dest); //assuming dest is properly null terminated
     while (i < n && src[i] != '\0') {
         dest[destLen + i] = src[i];
         i++;
     }
     dest[destLen + i] = '\0';

     return dest;
}

int mystrncmp(const char *s1, const char *s2, size_t n)
{
     /* If either dest or src are NULL return NULL */
     if(!s1 || !s2) return 0;
     /* if n is 0 we are done nothing compared to nothing is equal! */
     if(0 == n) return 0;

     size_t i = 0;
     int match = 1;
     while (match && i < n) {
         if (s1[i] == s2[i])
            i++;
         else
             match = 0;
     }

    if (match)
        return 0;
    else if (s1[i] > s2[i])
        return 1;
    else
        return -1;
}
