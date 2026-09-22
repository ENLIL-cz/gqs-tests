#include <openssl/evp.h>

#ifdef ENABLE_LEGACY_CRYPTO
static int legacy_cipher(void) {
    EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
    EVP_EncryptInit_ex(ctx, EVP_des_ede3_cbc(), NULL, NULL, NULL);
    EVP_CIPHER_CTX_free(ctx);
    return 1;
}
#endif

int main(void) {
    return 0;
}
