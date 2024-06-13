#ifndef LISKA_H
#define LISKA_H

#include <stdint.h>

void cifrar_liska(const char *datos, const char *key, char *datos_cifrados);
void decifrar_liska(const char *datos, const char *key, char *datos_decifrados);
void hexadecimal(const char *datos);

#endif // LISKA_H
