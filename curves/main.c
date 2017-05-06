#include "primeec.h"

int main (int argc, char ** argv)
{
    mpz_t parama;
    mpz_t paramb;
    mpz_t coef;
    mpz_t p;
    struct point a, b;
    mpz_inits (p, a.x, a.y, b.x, b.y, parama, paramb, coef, NULL);

    mpz_set_si (a.x, -1);
    mpz_set_si (a.y, -1);
    mpz_set_si (b.x, -1);
    mpz_set_si (b.y, -1);

    int index;
    int c;
    opterr = 0;
    while ((c = getopt (argc, argv, "x:y:w:z:p:m:a:b:")) != -1) {
        switch (c) {
            case 'x':
                mpz_set_str (a.x, optarg, 16);
                break;
            case 'y':
                mpz_set_str (a.y, optarg, 16);
                break;
            case 'w':
                mpz_set_str (b.x, optarg, 16);
                break;
            case 'z':
                mpz_set_str (b.y, optarg, 16);
                break;
            case 'p':
                mpz_set_str (p, optarg, 16);
                break;
            case 'm':
                mpz_set_str (coef, optarg, 16);
                break;
            case 'a':
                mpz_set_str (parama, optarg, 16);
                break;
            case 'b':
                mpz_set_str (paramb, optarg, 16);
                break;
            case '?':
                if (optopt == 'x' || optopt == 'y' || optopt == 'w' ||
                    optopt == 'z' || optopt == 'p' || optopt == 'm' ||
                    optopt == 'a' || optopt == 'b') {
                    fprintf (stderr, "Option -%c requires an argument.\n", optopt);
                }
                else if (isprint (optopt)) {
                    fprintf (stderr, "Unknown option: -%c\n", optopt);
                }
                else {
                    fprintf (stderr, "Unknown option char: \\x%x\n", optopt);
                }
                exit (1);
            default:
                fprintf (stderr, "Something went wrong with opts\n");
                exit(2);
        }
    }
/*
    if (mpz_cmp_si (a.x, -1) < 0) {
        fprintf (stderr,"The x coordinate of the first point must be positive\n");
        exit(1);
    }
    if (mpz_cmp_si (a.y, -1) < 0) {
        fprintf (stderr,"The y coordinate of the first point must be positive\n");
        exit(1);
    }
    if (mpz_cmp_si (b.x, 0) < -1) {
        fprintf (stderr,"The x coordinate of the second point must be positive\n");
        exit(1);
    }
    if (mpz_cmp_si (b.y, 0) < -1) {
        fprintf (stderr,"The y coordinate of the second point must be positive\n");
        exit(1);
    }
*/
    if (mpz_cmp_si (p, 0) < 0) {
        fprintf (stderr, "The field modulus must be positive");
        exit(1);
    }

    if (!isPointOnCurve(a, p, parama, paramb)) {
        gmp_printf ("first provided point does not lie on the curve with a = %Zx and b = %Zx\n", parama, paramb);
        exit (3);
    }
    if (!isPointOnCurve(b, p, parama, paramb)) {
        gmp_printf ("second provided point does not lie on the curve with a = %Zx and b = %Zx\n", parama, paramb);
        exit (4);
    }

    struct point r;
    mpz_inits (r.x, r.y, NULL);

    if (mpz_cmp_si (coef, 0) == 0) {
        pAddition (&r, a, b, p, parama);
    }
    if (mpz_cmp_si(coef,0) > 0) {
        pMul (&r, a, coef, p, parama);
    }

    gmp_printf ("x: %Zx\ny: %Zx\n", r.x, r.y);

    mpz_clears (p, a.x, a.y, b.x, b.y, r.x, r.y, parama, paramb, coef, NULL);
}
