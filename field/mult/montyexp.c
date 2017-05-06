#include <gmp.h>
#include <stdio.h>
#include <stdlib.h>

int main (int argc, char * argv[])
{
    if (argc != 3) {
        printf ("USAGE: montyexp <base> <exponent>\n");
        exit (1);
    }

    mpz_t x, xb, p, r, ri, pp, a, normal;
    mpz_init_set_str (x, argv[1], 10);
    mpz_init_set_str (p, "1794258236543787", 10);
    mpz_init_set_str (r, "18446744073709551616", 10);
    mpz_inits (pp, xb, a, ri, normal, NULL);

    unsigned int e = strtol(argv[2], NULL, 10);
    unsigned int pow = e;

    mpz_invert (pp, p, r);
    mpz_sub (pp, r, pp);
    mpz_invert (ri, r, p);

    mpz_mul (xb, x, r);
    mpz_mod (xb, xb, p);
    mpz_mod (a, r, p);

    int shifts = 31;

    while (!(e & 0x80000000)) {
        printf ("e: %d\n", e);
        e = e << 1;
        shifts --;
    }

    for (shifts; shifts >= 0; shifts--) {
        printf ("e2: %d\n", e);
        mpz_mul (a, a, a);
        mpz_mul (a, ri, a);
        mpz_mod (a, a, p);

        if (e & 0x80000000) {
            mpz_mul (a, a, xb);
            mpz_mul (a, a, ri);
            mpz_mod (a, a, p);
        }

        e = e << 1;
    }

    mpz_mul (a, ri, a);
    mpz_mod (a, a, p);

    mpz_powm_ui (normal, x, pow, p);

    gmp_printf ("%s^%s  = %Zd\n", argv[1], argv[2], a);
    gmp_printf ("Normal= %Zd\n", normal);


    mpz_clears (x, xb, p, r, ri, pp, a, NULL);
}
