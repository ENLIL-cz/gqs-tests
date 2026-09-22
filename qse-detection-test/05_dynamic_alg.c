#include <openssl/evp.h>
#include <stdio.h>

int main(int argc, char **argv) {
    if (argc < 2) return 1;
    const EVP_CIPHER *c = EVP_get_cipherbyname(argv[1]);
    if (!c) return 1;
    EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
    EVP_EncryptInit_ex(ctx, c, NULL, NULL, NULL);
    EVP_CIPHER_CTX_free(ctx);
    return 0;
}
