//////////////////////////////////////////////////////////////////////////////////////////////
// Riley Cooper
// 19 March 2017
// Elliptic Curve Crypto Independent Study
// This file implements random elliptic curve generation and validation.  The idea is to
//      generate EC's randomly to avoid possible future attacks against EC's with specific
//      forms.  This also includes a validation function to make ensure security of the
//      generated EC.  This program shall use SHA512 as its hash algorithm.
//

#include <openssl/evp.h>
#include <stdlib.h>
#include <stdio.h>
#include <gmp.h>
#include <math.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define HASHBITS 512

//This function takes an int and converts it into a character with the corresponding
//      hex value as a character
char itoshexmap (int x)
{
    switch (x) {
        case 0:
            return '0';
            break;
        case 1:
            return '1';
            break;
        case 2:
            return '2';
            break;
        case 3:
            return '3';
            break;
        case 4:
            return '4';
            break;
        case 5:
            return '5';
            break;
        case 6:
            return '6';
            break;
        case 7:
            return '7';
            break;
        case 8:
            return '8';
            break;
        case 9:
            return '9';
            break;
        case 10:
            return 'a';
            break;
        case 11:
            return 'b';
            break;
        case 12:
            return 'c';
            break;
        case 13:
            return 'd';
            break;
        case 14:
            return 'e';
            break;
        case 15:
            return 'f';
            break;
        default:
            return -1;
    }
}

void genRandEC (char *S, int *a, int *b, mpz_t p)
{
printf ("START\n");
    size_t t = mpz_sizeinbase (p, 2);
    unsigned long s = floor ((t-1)/HASHBITS);
    unsigned long v = t - s*HASHBITS;

printf ("before get seed\n");
    // Get a random seed not based on time to use for generating a random EC seed
    int fd = open("/dev/urandom", O_RDONLY);
    long int seed = 0;
    read (fd, &seed, sizeof(seed));
    close (fd);
    srand48(seed);
printf ("Got seed\n");

    // Generate the random EC seed and store it in S
    int length = lrand48() % 128;
    int i = 0;
    for (i; i < length + 128; i++) {
        S[i] = itoshexmap(lrand48() % 256);
    }
printf ("generated seed\n");

    //Compute the SHA512 hash of S
    unsigned char  digest[129];
    memset (digest, 0, 129);
    unsigned int * digest_len = malloc(sizeof (unsigned int));
    *digest_len = 128;
    EVP_MD_CTX *mdctx;
    mdctx = EVP_MD_CTX_create();
    EVP_DigestInit_ex (mdctx, EVP_sha512(), NULL);
printf("test0\n");
    EVP_DigestUpdate (mdctx, S, length);
printf("test1\n");
//    *digest = (unsigned char *) OPENSSL_malloc(EVP_MD_size(EVP_sha512()));
printf("test2\n");
    EVP_DigestFinal_ex (mdctx, digest, digest_len);
printf("test3\n");
    EVP_MD_CTX_destroy (mdctx);
printf("test4\n");
    free (digest_len);
printf("len: %d\n", strlen(digest));
    printf ("digest: %s\n", digest);
}

int main ()
{
    char buffer[256];
    memset (buffer, 0, 256);
    mpz_t p;
    mpz_init (p);
    mpz_set_ui (p, 8831);
    int a, b;
    genRandEC (buffer, &a, &b, p);
    mpz_clear(p);
}
