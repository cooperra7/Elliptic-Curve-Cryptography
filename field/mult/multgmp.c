#include <gmp.h>
#include <stdio.h>
#include <stdlib.h>

int main ()
{
    mpz_t a, b, result;

    mpz_init_set_str (a, "12345678900987654321", 10);
    mpz_init_set_str (b, "98765432100123456789", 10);

    mpz_init (result);

    mpz_mul (result, a, b);

    gmp_printf ("Result: %Zd\n", result);

    mpz_clears (a, b, result, NULL);
}
