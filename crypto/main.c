#include "mycryptosuite.h"



int main (int argc, char ** argv)
{
    struct domain_params params;
    mpz_inits (params.field_order, params.seed, params.parama, params.paramb, params.base_point.x, params.base_point.y, params.base_point_order, params.cofactor, NULL);
    set_domain_params (&params, 256);
    struct point pub;
    mpz_t priv;
    mpz_init (priv);
    mpz_init (pub.x);
    mpz_init (pub.y);

    keygen (&pub, &priv, params);

    gmp_printf ("PubX: %Zx\nPubY: %Zx\nPriv: %Zx\n", pub.x, pub.y, priv);

    struct ecdsa_sig sig;
    mpz_inits (sig.r, sig.s, NULL);

char message[12];
memset (message, 0, 12);
strcpy (message, "Hello Riley");
    
    sign_ecdsa (&sig, params, priv, message);
    gmp_printf ("Hello World:\n\tr: %Zx\n\ts: %Zx\n\n", sig.r, sig.s);

    if (verify_ecdsa (sig, params, pub, message)) printf ("SUCCESS\n");
    else printf("FAIL\n");
    mpz_clears (params.field_order, params.seed, params.parama, params.paramb, params.base_point.x, params.base_point.y, params.base_point_order, params.cofactor, NULL);
    mpz_clear (pub.x);
    mpz_clear (pub.y);
    mpz_clear (priv);
    mpz_clears (sig.r, sig.s, NULL);
}
