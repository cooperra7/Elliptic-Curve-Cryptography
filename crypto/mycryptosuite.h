#ifndef ECC_CRYPTO_H
#define ECC_CRYPTO_H
#include <gmp.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <openssl/sha.h>
#include "primeec.h"

struct domain_params
{
    mpz_t field_order;
    bool prime_field;
    mpz_t seed;
    mpz_t parama;
    mpz_t paramb;
    struct point base_point;
    mpz_t base_point_order;
    mpz_t cofactor;
};

struct ecdsa_sig
{
    mpz_t r;
    mpz_t s;
};

void keygen (struct point * pub, mpz_t * priv, const struct domain_params params);

bool pub_key_validation (const struct domain_params params, const struct point pub);

void set_domain_params (struct domain_params * params, int strength);

//void encrypt_ecies (struct ecies_cipher * out, const struct domain_params params, const point pub, char * message);

//void decrypt_ecies (char * plain, const struct domain_params params, const mpz_t priv, const struct ecies_cipher cipher);

void sign_ecdsa (struct ecdsa_sig * sig, const struct domain_params params, const mpz_t priv, char * message);

bool verify_ecdsa (const struct ecdsa_sig sig, const struct domain_params params, const struct point pub, char * message);

#endif
