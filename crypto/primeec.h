#ifndef PRIME_EC_H
#define PRIME_EC_H

#include <stdio.h>
#include <gmp.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>

struct point
{
    mpz_t x;
    mpz_t y;
};

bool isPointOnCurve (const struct point q, const mpz_t p, const mpz_t a, const mpz_t b);

void aSlope (mpz_t * r, const struct point a, const struct point b, const mpz_t p);

void dSlope (mpz_t * r, const struct point a, const mpz_t p, const mpz_t param);

void pAddition (struct point * r, const struct point a, const struct point b, const mpz_t p, const mpz_t param);

void pMul (struct point * r, const struct point a, const mpz_t coef, const mpz_t p, const mpz_t param);

#endif
