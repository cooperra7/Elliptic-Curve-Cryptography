#include <gmp.h>
#include <stdlib.h>
#include <stdio.h>

int main ()
{
    mpz_t x;
    mpz_init_set_str (x, "1", 10);
    mpz_neg (x, x);
    gmp_printf ("x: %Zd\n", x);
    gmp_printf ("x: %Zu\n", x);
    mpz_clear (x);
}
