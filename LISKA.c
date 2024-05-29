#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

uint64_t xorshift(const char *key, size_t key_tam) { // idea de xorshift*
    uint64_t k = 0;
    for (size_t i = 0; i < key_tam; i++) {
        k ^= (uint64_t)(key[i] + 1);
    }
    k ^= k << 31;
    k ^= k >> 11;
    k ^= k ^ (k >> 30);
    return k;
}

uint64_t split_mix(uint64_t s) { // idea de xoroshiro
    s = (s += 0x9E3779B97F4A7C15);
    s = (s ^ (s >> 30)) * 0xBF58476D1CE4E5B9;
    s = (s ^ (s >> 27)) * 0x95D049BB133111EB;
    return (s ^ (s >> 31));
}

void cifrar_LISKA(const char *datos, const char *key, char *datos_cifrados) {
    uint8_t key_byte, c, x;
    uint64_t key_xorshift = xorshift(key, strlen(key));
    uint64_t s = key_xorshift;

    for (size_t i = 0; i <= strlen(datos); i++) {
        s = split_mix(s);
        s = (s >> (s % 32)) | (s << ((-s % 32) & 31)); 
        key_byte = key[i % strlen(key)];
        x = datos[i];
        c = ((x + 0x88 + key_byte) & 0xFF);
        c = ((c << (s % 8)) | (c >> (8 - s % 8))) & 0xFF;
        c ^= (s & 0xFF);
        c = ((c << (key_byte % 8)) | (c >> (8 - key_byte % 8))) & 0xFF;
        datos_cifrados[i] = c;
    }
    datos_cifrados[strlen(datos)] = '\0';
}

void decifrar_LISKA(const char *datos, const char *key, char *datos_decifrados) {
    uint8_t key_byte, c;
    uint64_t key_xorshift = xorshift(key, strlen(key));
    uint64_t s = key_xorshift;

    for (size_t i = 0; i <= strlen(datos); i++) {
        s = split_mix(s);
        s = (s >> (s % 32)) | (s << ((-s % 32) & 31));
        key_byte = key[i % strlen(key)];
        c = datos[i];
        c = ((c >> (key_byte % 8)) | (c << (8 - key_byte % 8))) & 0xFF;
        c ^= (s & 0xFF);
        c = ((c >> (s % 8)) | (c << (8 - s % 8))) & 0xFF;
        c = ((c - 0x88 - key_byte) & 0xFF);
        datos_decifrados[i] = c;
    }
    datos_decifrados[strlen(datos)] = '\0';
}

void hexadecimal(const char *datos) {
    while (*datos) {
        printf("%02X", (unsigned char)*datos);
        datos++;
    }
    printf("\n");
}

int main() {
    const char *texto = "Wired";
    const char *key = "Lain";
    char *datos_cifrados = (char *)malloc(strlen(texto));
    char *datos_decifrados = (char *)malloc(strlen(texto));

    cifrar_LISKA(texto, key, datos_cifrados);
    printf("Cifrado: ");
    hexadecimal(datos_cifrados);

    decifrar_LISKA(datos_cifrados, key, datos_decifrados);
    printf("Descifrado: %s\n", datos_decifrados);
    return 0;
}
