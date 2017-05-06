#include "mycryptosuite.h"


char maptohex (int val)
{
    switch (val) {
    case (0): {
        return '0';
        break; }
    case (1): {
        return '1';
        break; }
    case (2): {
        return '2';
        break; }
    case (3): {
        return '3';
        break; }
    case (4): {
        return '4';
        break; }
    case (5): {
        return '5';
        break; }
    case (6): {
        return '6';
        break; }
    case (7): {
        return '7';
        break; }
    case (8): {
        return '8';
        break; }
    case (9): {
        return '9';
        break; }
    case (10): {
        return 'a';
        break; }
    case (11): {
        return 'b';
        break; }
    case (12): {
        return 'c';
        break; }
    case (13): {
        return 'd';
        break; }
    case (14): {
        return 'e';
        break; }
    case (15): {
        return 'f';
        break; }
    default:
        return '0';
    }
}

bool mysha256 (mpz_t * val, void * in, unsigned int length)
{
    unsigned char md[SHA256_DIGEST_LENGTH];
    unsigned char toval[SHA256_DIGEST_LENGTH * 2+1];
    SHA256_CTX context;
    if(!SHA256_Init(&context))
        return false;

    if(!SHA256_Update(&context, (unsigned char*)in, length))
        return false;

    if(!SHA256_Final(md, &context))
        return false;

    int i = 0;
    for (i; i < SHA256_DIGEST_LENGTH * 2; i+=2) {
        toval[i] =  maptohex ((unsigned int) md[i/2] % 0xf);
        toval[i+1] =  maptohex ((unsigned int) md[i/2] / 0xf);
    }
    toval[SHA256_DIGEST_LENGTH * 2] = 0;
    mpz_set_str (*val, toval + 16, 16);
    return true;
}

void readurandom (mpz_t * rand, const mpz_t max)
{
    unsigned char res;
    unsigned int size = mpz_sizeinbase (max, 256);
    unsigned char randbuffer[size*2];

    mpz_t temp, randtest, mpzres;
    mpz_init(temp);
    mpz_init(randtest);
    mpz_init (mpzres);
    mpz_set (temp, max);
    mpz_set_ui (randtest, 0xff);

    int fd = open ("/dev/urandom", O_RDONLY);
    int i = 0;
    while (mpz_cmp_ui (temp, 0) > 0) {
        read (fd, &res, sizeof(unsigned char));
        mpz_and (mpzres, temp, randtest);
        mpz_tdiv_q_2exp (temp, temp, 8);
        unsigned char chartemp;
        if (mpz_cmp_ui(mpzres, 0) != 0) {
            chartemp = (unsigned char) (res % mpz_get_ui (mpzres));
        }
        else {
            chartemp = (unsigned char) 0;
        }
        randbuffer[size * 2 - 1 - i] = maptohex ((unsigned int) chartemp % 0x0f);
        i++;
        randbuffer[size * 2 - 1 - i] = maptohex ((unsigned int) chartemp / 0x0f);
        i++;
    }

    randbuffer[size*2] = (unsigned char) 0;
    mpz_set_str (*rand, randbuffer, 16);
    close (fd);
    mpz_clears (temp, randtest, mpzres, NULL);
}

void keygen (struct point * pub, mpz_t * priv, const struct domain_params params)
{
    readurandom(priv, params.base_point_order);
    do {
        pMul (pub, params.base_point, *priv, params.field_order, params.parama);
    } while (!pub_key_validation (params, *pub));
}

bool pub_key_validation (const struct domain_params params, const struct point pub)
{
    return isPointOnCurve (pub, params.field_order, params.parama, params.paramb);
}

void set_domain_params (struct domain_params * params, int strength)
{
    switch (strength) {
    case 192: {
        mpz_set_str (params->field_order, "FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEFFFFFFFFFFFFFFFF", 16);
        params->prime_field = true;
        mpz_set_ui (params->seed, 0);
        mpz_set_str (params->parama, "FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEFFFFFFFFFFFFFFFC", 16);
        mpz_set_str (params->paramb, "64210519E59C80E70FA7E9AB72243049FEB8DEECC146B9B1", 16);
        mpz_set_str (params->base_point.x, "188DA80EB03090F67CBF20EB43A18800F4FF0AFD82FF1012", 16);
        mpz_set_str (params->base_point.y, "07192B95FFC8DA78631011ED6B24CDD573F977A11E794811", 16);
        mpz_set_str (params->base_point_order, "FFFFFFFFFFFFFFFFFFFFFFFF99DEF836146BC9B1B4D22831", 16);
        mpz_set_ui (params->cofactor, 1);
        break;
    }
    case 224: {
        mpz_set_str (params->field_order, "FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF000000000000000000000001", 16);
        params->prime_field = true;
        mpz_set_ui (params->seed, 0);
        mpz_set_str (params->parama, "FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEFFFFFFFFFFFFFFFFFFFFFFFE", 16);
        mpz_set_str (params->paramb, "B4050A850C04B3ABF54132565044B0B7D7BFD8BA270B39432355FFB4", 16);
        mpz_set_str (params->base_point.x, "B70E0CBD6BB4BF7F321390B94A03C1D356C21122343280D6115C1D21", 16);
        mpz_set_str (params->base_point.y, "BD376388B5F723FB4C22DFE6CD4375A05A07476444D5819985007E34", 16);
        mpz_set_str (params->base_point_order, "FFFFFFFFFFFFFFFFFFFFFFFFFFFF16A2E0B8F03E13DD29455C5C2A3D", 16);
        mpz_set_ui (params->cofactor, 1);
        break;
    }
    case 256: {
        mpz_set_str (params->field_order, "FFFFFFFF00000001000000000000000000000000FFFFFFFFFFFFFFFFFFFFFFFF", 16);
        params->prime_field = true;
        mpz_set_ui (params->seed, 0);
        mpz_set_str (params->parama, "FFFFFFFF00000001000000000000000000000000FFFFFFFFFFFFFFFFFFFFFFFC", 16);
        mpz_set_str (params->paramb, "5AC635D8AA3A93E7B3EBBD55769886BC651D06B0CC53B0F63BCE3C3E27D2604B", 16);
        mpz_set_str (params->base_point.x, "6B17D1F2E12C4247F8BCE6E563A440F277037D812DEB33A0F4A13945D898C296", 16);
        mpz_set_str (params->base_point.y, "4FE342E2FE1A7F9B8EE7EB4A7C0F9E162BCE33576B315ECECBB6406837BF51F5", 16);
        mpz_set_str (params->base_point_order, "FFFFFFFF00000000FFFFFFFFFFFFFFFFBCE6FAADA7179E84F3B9CAC2FC632551", 16);
        mpz_set_ui (params->cofactor, 1);
        break;
    }
    case 384: {
        mpz_set_str (params->field_order, "FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEFFFFFFFF0000000000000000FFFFFFFF", 16);
        params->prime_field = true;
        mpz_set_ui (params->seed, 0);
        mpz_set_str (params->parama, "FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEFFFFFFFF0000000000000000FFFFFFFC", 16);
        mpz_set_str (params->paramb, "B3312FA7E23EE7E4988E056BE3F82D19181D9C6EFE8141120314088F5013875AC656398D8A2ED19D2A85C8EDD3EC2AEF", 16);
        mpz_set_str (params->base_point.x, "AA87CA22BE8B05378EB1C71EF320AD746E1D3B628BA79B9859F741E082542A385502F25DBF55296C3A545E3872760AB7", 16);
        mpz_set_str (params->base_point.y, "3617DE4A96262C6F5D9E98BF9292DC29F8F41DBD289A147CE9DA3113B5F0B8C00A60B1CE1D7E819D7A431D7C90EA0E5F", 16);
        mpz_set_str (params->base_point_order, "FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFC7634D81F4372DDF581A0DB248B0A77AECEC196ACCC52973", 16);
        mpz_set_ui (params->cofactor, 1);
        break;
    }
    case 521: {
        mpz_set_str (params->field_order, "1FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF", 16);
        params->prime_field = true;
        mpz_set_ui (params->seed, 0);
        mpz_set_str (params->parama, "1FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFC", 16);
        mpz_set_str (params->paramb, "51953EB9618E1C9A1F929A21A0B68540EEA2DA725B99B315F3B8B489918EF109E156193951EC7E937B1652C0BD3BB1BF073573DF883D2C34F1EF451FD46B503F00", 16);
        mpz_set_str (params->base_point.x, "C6858E06B70404E9CD9E3ECB662395B4429C648139053FB521F828AF606B4D3DBAA14B5E77EFE75928FE1DC127A2FFA8DE3348B3C1856A429BF97E7E31C2E5BD66", 16);
        mpz_set_str (params->base_point.y, "11839296A789A3BC0045C8A5FB42C7D1BD998F54449579B446817AFBD17273E662C97EE72995EF42640C550B9013FAD0761353C7086A272C24088BE94769FD16650", 16);
        mpz_set_str (params->base_point_order, "1FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFA51868783BF2F966B7FCC0148F709A5D03BB5C9B8899C47AEBB6FB71E91386409", 16);
        mpz_set_ui (params->cofactor, 1);
        break;
    }
    default: {
    }
    }
}

void sign_ecdsa (struct ecdsa_sig * sig, const struct domain_params params, const mpz_t priv, char * message)
{
    mpz_t k, e;
    struct point kP;
    mpz_init (kP.x);
    mpz_init (kP.y);
    mpz_init (k);
    mpz_init (e);
    mpz_set_ui (sig->r, 0);
    mpz_set_ui (sig->s, 0);
    while (mpz_cmp_ui (sig->s, 0) == 0) {
        while (mpz_cmp_ui (sig->r, 0) == 0) {
            readurandom(&k, params.base_point_order);
            pMul (&kP, params.base_point, k, params.field_order, params.parama);
            mpz_mod (sig->r,kP.x, params.base_point_order);
        }
        mysha256 (&e, message, strlen(message));
        mpz_invert (k, k, params.base_point_order);
        mpz_mul (kP.y, priv, sig->r);
        mpz_mod (kP.y, kP.y, params.base_point_order);
        mpz_add (kP.x, e, kP.y);
        mpz_mul (sig->s, kP.x, k);
        mpz_mod (sig->s, sig->s, params.base_point_order);
    }
    mpz_clears(kP.x, kP.y, k, e, NULL);
}

bool verify_ecdsa (const struct ecdsa_sig sig, const struct domain_params params, const struct point pub, char * message)
{
    if (mpz_cmp (sig.r, params.base_point_order) >= 0 || mpz_cmp_ui (sig.r, 1) < 0) {
        printf ("r out of range\n");
        return false;
    }
    if (mpz_cmp (sig.s, params.base_point_order) >= 0 || mpz_cmp_ui (sig.s, 1) < 0) {
        printf ("s out of range\n");
        return false;
    }
    mpz_t e, w, u1, u2, v;
    struct point x;
    struct point temp;
    mpz_inits (e, w, u1, u2, v, x.x, x.y, temp.x, temp.y, NULL);

    mysha256 (&e, message, strlen(message));
    mpz_invert (w, sig.s, params.base_point_order);
    mpz_mod (w, w, params.base_point_order);
    mpz_mul (u1, e, w);
    mpz_mul (u2, sig.r, w);
    mpz_mod (u1, u1, params.base_point_order);
    mpz_mod (u2, u2, params.base_point_order);
    pMul (&temp, params.base_point, u1, params.field_order, params.parama);
    pMul (&x, pub, u2, params.field_order, params.parama);
    pAddition (&x, x, temp, params.field_order, params.parama);
    if (mpz_cmp_si (x.x, -1) == 0 && mpz_cmp_si (x.y, -1) == 0) {
        printf ("x is point at infinity\n");
        mpz_clears (e, w, u1, u2, v, x.x, x.y, temp.x, temp.y, NULL);
        return false;
    }
    mpz_mod (v, x.x, params.base_point_order);
    if (mpz_cmp (v,sig.r) == 0) {
        mpz_clears (e, w, u1, u2, v, x.x, x.y, temp.x, temp.y, NULL);
        return true;
    }
    mpz_clears (e, w, u1, u2, v, x.x, x.y, temp.x, temp.y, NULL);
    return false;
}
