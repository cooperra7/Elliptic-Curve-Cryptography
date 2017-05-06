#include <gmp.h>
#include <stdio.h>
#include <stdlib.h>

int main ()
{
    mpz_t a;
    mpz_t b;
    mpz_t c;
    mpz_inits (a, b, c, NULL);

    mp_limb_t d[2];

    mpz_set_str (a, "11111111111111111111111111111111", 16);
    mpz_set_str (b, "11111111111111111111111111111111", 16);
    mpz_set_str (c, "11111111111111111111111111111111", 16);

    printf ("bits per limb: %d\n\n", mp_bits_per_limb);

    mp_limb_t alimb = mpz_getlimbn (a,0);
    mp_size_t asize = mpz_size (a);
    gmp_printf ("alimb: %Mx\n", alimb);
    gmp_printf ("asize: %d\n\n", asize);

    mp_limb_t blimb = mpz_getlimbn (b,0);
    mp_size_t bsize = mpz_size (b);
    gmp_printf ("blimb: %Mx\n", blimb);
    gmp_printf ("bsize: %d\n\n", bsize);

    _mpz_realloc (c, 2);
    mp_size_t csize = mpz_size (c);
    gmp_printf ("csize: %d\n", csize);
    mp_limb_t climb = mpz_getlimbn (c,0);

    mpn_add (d, &alimb, asize, &blimb, bsize);
    gmp_printf ("Low Result: %Mx\n", d[0]);

    mpz_add (c, a, b);
    gmp_printf ("High Result: %Zx\n", c);
    csize = mpz_size (c);
    gmp_printf ("csize: %d\n", csize);

    mpz_clears (a, b, c, NULL);
}
