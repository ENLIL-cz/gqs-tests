#include <stdio.h>

/* Historicka poznamka: driv se tady pouzivalo RSA-2048 a SHA-1.
   Migrace na ML-KEM je naplanovana. */

int main(void) {
    const char *legacy_note = "RSA-2048 / ECDSA-P256 / MD5";
    printf("config profile: AES-256-GCM, TLS_RSA_WITH_AES_128_CBC_SHA\n");
    printf("%s\n", legacy_note);
    return 0;
}
