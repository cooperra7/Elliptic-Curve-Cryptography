#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define BITLENGTH 128
#define WORDLENGTH 4

bool multiprecisionadd (unsigned int * a, unsigned int * b, unsigned int * c, int size)
{
    bool carry = false;
    c[0] = a[0] + b[0];
    if (c[0] < a[0] || c[0] < b[0]) {
        carry = true;
    }

    int i = 1;
    for (i; i < size; i=i+1) {
        c[i] = a[i] + b[i] + (int) carry;
        if (c[i] < a[i] || c[i] < b[i]) {
            carry = true;
        }
        else {
            carry = false;
        }
    }
    return carry;
}

int main (int argc, char * argv[])
{
    unsigned int a[WORDLENGTH];
    unsigned int b[WORDLENGTH];
    unsigned int c[WORDLENGTH];

    int i = 0;

    for (i; i < WORDLENGTH; i=i+1) {
        a[i] = 0;
        b[i] = 0;
        c[i] = 0;
    }

    bool carry = false;

    a[0] = 4294967295;
    b[0] = 1;

    carry = multiprecisionadd (a,b,c,WORDLENGTH);

    printf ("carry: %d\n", carry);
    for (i=0; i < WORDLENGTH; i=i+1) {
        printf ("val[%d]: %d\n", i, c[i]);
    }
}
