#include <openssl/evp.h>

/* Tato funkce neni volana odnikud. */
static int unused_legacy_keygen(EVP_PKEY **out) {
    EVP_PKEY_CTX *ctx = EVP_PKEY_CTX_new_id(EVP_PKEY_RSA, NULL);
    if (!ctx) return 0;
    EVP_PKEY_keygen_init(ctx);
    EVP_PKEY_CTX_set_rsa_keygen_bits(ctx, 1024);
    EVP_PKEY_keygen(ctx, out);
    EVP_PKEY_CTX_free(ctx);
    return 1;
}

int main(void) {
    return 0;
}
