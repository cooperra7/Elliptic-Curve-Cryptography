#include <stdio.h>
#include <gmp.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include "primeec.h"

/*
 * This function takes a point, the size of the prime field, and the parameters for the elliptic
 * curve and checks that the point lies on the elliptic curve.
 */
bool isPointOnCurve (const struct point q, const mpz_t p, const mpz_t a, const mpz_t b)
{
    mpz_t left, right, ax;
    mpz_inits (left, right, ax, NULL);
    mpz_powm_ui (left, q.y, 2, p);
    mpz_powm_ui (right, q.x, 3, p);
    mpz_mul (ax, q.x, a);
    mpz_add (right, right, ax);
    mpz_add (right, right, b);
    mpz_mod (left, left, p);
    mpz_mod (right, right, p);
    bool ret = false;
    if (mpz_cmp (left, right) == 0 && mpz_cmp_si (q.x, 0) >= 0 && mpz_cmp_si (q.y, 0) >= 0) {
        ret = true;
    }
    if (!mpz_cmp_si(q.x, -1) && !mpz_cmp_si(q.y, -1)) {
        ret = true;
    }
    mpz_clears (left, right, ax, NULL);
    return ret;
}

/*
 * This function takes two points and calculates the slope between them for point addition between
 * two different points. It calculates delta y over delta x
 */
void aSlope (mpz_t * r, const struct point a, const struct point b, const mpz_t p)
{
    mpz_t ydiff, xdiff;
    mpz_inits (ydiff, xdiff, NULL);
    mpz_sub(xdiff, a.x, b.x);
    mpz_sub(ydiff, a.y, b.y);
    mpz_invert (xdiff, xdiff, p);
    mpz_mul (*r, xdiff, ydiff);
    mpz_mod (*r, *r, p);
    mpz_clears(ydiff, xdiff, NULL);
}

/*
 * This function takes one point and calculates the slope of the tangent line at that point on
 * the elliptic curve.  This determines the slope to use when the two addition points are the same.
 */
void dSlope (mpz_t * r, const struct point a, const mpz_t p, const mpz_t param)
{
    mpz_t xtemp, ytemp;
    mpz_inits (xtemp, ytemp, NULL);
    mpz_mul (xtemp, a.x, a.x);
    mpz_mul_ui (xtemp, xtemp, 3);
    mpz_add (xtemp, xtemp, param);
    mpz_mul_ui (ytemp, a.y, 2);
    mpz_invert (ytemp, ytemp, p);
    mpz_mul (*r, xtemp, ytemp);
    mpz_mod (*r, *r, p);
    mpz_clears (xtemp, ytemp, NULL);
}

/*
 * This function takes two points and adds them together and returns the result
 */
void pAddition (struct point * r, const struct point a, const struct point b, const mpz_t p, const mpz_t param)
{
    mpz_t temp, m, newx, newy;
    mpz_inits (temp, m, newx, newy, NULL);
    if (mpz_cmp(a.x, b.x) == 0 && mpz_cmp(a.y, b.y) == 0) {
        dSlope (&m, a, p, param);
    }
    else {
        aSlope (&m, a, b, p);
    }
    mpz_mul (temp, m, m);
    mpz_sub (temp, temp, a.x);
    mpz_sub (temp, temp, b.x);
    mpz_mod (newx, temp, p);
    mpz_sub (temp, newx, a.x);
    mpz_mul (temp, temp, m);
    mpz_add (temp, a.y, temp);
    mpz_neg (temp, temp);
    mpz_mod (newy, temp, p);
    mpz_sub (temp, p, a.y);
//    mpz_neg (temp, a.y);
//    mpz_mod (temp,temp, p);
    // Checks placed here rather than beginning to avoid time based side channel
    if (!mpz_cmp (temp, b.y) && !mpz_cmp (a.x, b.x)) {
        mpz_set_si (r->x, -1);
        mpz_set_si (r->y, -1);
        mpz_clears (temp, m, newx, newy, NULL);
        return;
    }
    if (!mpz_cmp_si (a.x, -1) && !mpz_cmp_si(a.y, -1)) {
        mpz_set (r->x, b.x);
        mpz_set (r->y, b.y);
        mpz_clears (temp, m, newx, newy, NULL);
        return;
    }
    if (!mpz_cmp_si (b.x, -1) && !mpz_cmp_si(b.y, -1)) {
        mpz_set (r->x, a.x);
        mpz_set (r->y, a.y);
        mpz_clears (temp, m, newx, newy, NULL);
        return;
    }
    mpz_set (r->x, newx);
    mpz_set (r->y, newy);
    mpz_clears (temp, m, newx, newy, NULL);
}

/*
 * This function uses a left to right scanning algorithm to multiply a point by a constant
 */
void pMul (struct point * r, const struct point a, const mpz_t coef, const mpz_t p, const mpz_t param)
{
    struct point q, temp;
    mpz_inits (q.x, q.y, temp.x, temp.y, NULL);
    mpz_set_si (q.x, -1);
    mpz_set_si (q.y, -1);
    mpz_set (temp.x, a.x);
    mpz_set (temp.y, a.y);
    unsigned int i = 0;
    unsigned int size = mpz_sizeinbase (coef, 2);
    while (i <= size) {
        if (mpz_tstbit (coef, i)) {
            pAddition (&q, q, temp, p, param);
        }
// Add an else to avoid side channel?
        pAddition (&temp, temp, temp, p, param);
        i++;
    }
    mpz_set (r->x, q.x);
    mpz_set (r->y, q.y);
    mpz_clears (q.x, q.y, temp.x, temp.y, NULL);
}
