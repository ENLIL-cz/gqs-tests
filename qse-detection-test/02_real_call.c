#include <openssl/evp.h>

int generate_key(EVP_PKEY **out) {
    EVP_PKEY_CTX *ctx = EVP_PKEY_CTX_new_id(EVP_PKEY_RSA, NULL);
    if (!ctx) return 0;
    EVP_PKEY_keygen_init(ctx);
    EVP_PKEY_CTX_set_rsa_keygen_bits(ctx, 2048);
    EVP_PKEY_keygen(ctx, out);
    EVP_PKEY_CTX_free(ctx);
    return 1;
}

int main(void) {
    EVP_PKEY *pkey = NULL;
    generate_key(&pkey);
    EVP_PKEY_free(pkey);
    return 0;
}
