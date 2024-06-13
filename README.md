<p align="center">
  <img src="https://github.com/ic4rta/LISKA/assets/blob/main/lain.png" alt="drawing" width="150"/>
</p>

<bold><p align="center">
  <strong>LISKA</strong>
</p></bold>

Algoritmo de cifrado simetrico que utiliza una longitud de llave variable

> [!WARNING]  
> Esto solo fue creado con el unico objetivo de aprender mas sobre cifrado simetrico, mas en concreto, cifrado de flujo, no se deberia de aplicar en un entorno real

## Algoritmos adicionales que se implementan

- **xorshift**: Es un PRNG el cual funciona haciendo XOR de una palabra de 64 bits (en este caso) con la version desplazada de si misma. En esta implementacion produce un valor pseudoaleatorio con la llave que ingresa el usuario
  
- **SplixMix**: Es el algoritmo que inicializa el estado de otro PRNG de nombre "xoroshiro/xoshiro", en este caso, se usa para mezclar aun mas el valor generado por xorshift

---

En la funcion cifrado, estas operaciones basicamente lo que hacen es inicializar el estado ```s``` de splitmix con el valor resultante de ```xorshift*```

```c
uint64_t key_xorshift = xorshift(key, strlen(key));
uint64_t s = key_xorshift;
```

## Proceso de cifrado

Primero se hace una desplazamiento a la derecha del resultado de calcular mod 32, el resultado es un desplazamieto de bits a la derecha del estado ```s```, posteriormente
se hace un desplazamiento a la izquierda de los bits de ```s```, del resultado de calcular el negativo modular de ```s``` y ```32```, y por ultimo
el resultado de los dos desplazamientos se realiza un ```OR``` a nivel de bits. Esto con el objetivo se agregar mas aleatoriedad en el estado ```s``` generado por ```SplitMix64```

```c
s = (s >> (s % 32)) | (s << ((-s % 32) & 31));
```

Posteriormente se selecciona un byte especifico de ```key``` en funcion de ```i```, para cifrar los datos de la posicion ```ì```, asi mismo, se calcula el modulo de ```i``` por la longitud de la llave ```key```,
esto con el fin de que ```i``` este en un rango valido de indices para ```key```.

```c
key_byte = key[i % strlen(key)]
```
Se asignan a ```x``` el valor del elemento en la posicion ```ì``` de ```datos```, que basicamente es cada byte del texto plano 

```c
x = datos[i];
```

Se realiza una suma de ```x```,  ```0x88``` y ```key_byte```, al resultado se le aplica un AND a nivel de bits sobre ```0xFF``` para garantizar que
el resultado este dentro de 8 bits, por lo que al realizar un AND, solo los bits en 1 menos significativos  de ```c``` se quedaran sin cambios, haciendo que solo se usen los 8 bits menos significativos en el caracter final,
ya que solo se necesita un byte para representar el caracter cifrado.

- C: contiene el byte cifrado (cada caracter)
- 0x88: es una constante

```c
c = ((x + 0x88 + key_byte) & 0xFF);
```

En la siguiente linea:

```c
c = ((c << (s % 8)) | (c >> (8 - s % 8))) & 0xFF;
```

- ```c << (s % 8)```: Se hace un desplazamiento a la izquierda de los bits cifrados del resultado del modulo del estado ```s``` por 8, para que el desplazamiento
este en un rango de 0 a 7 bits

- ```(c >> (8 - s % 8)```: Se hace un desplazamiento a la derecha de los bits cifrados del resultado del modulo ```s``` y por ```8```, y la resta de 8 - s. Toda la expresion
indica el numero de bits que se deben desplazar a la derecha para completar la rotacion de un byte

- A los resultados de las dos operaciones anteriores se le aplica un ```OR``` a nivel de bits

Posteriormente se realiza una operacion XOR entre ```c``` y con el bit correspondiente del resultado de ```s  & 0xFF```, en esta ultima operacion se asegura que
los 8 bits menos significativos de ```s``` se usen para el XOR

```c
c ^= (s & 0xFF);
```


Despues, la ultima operacion del cifrado donde

- ```(c << (key_byte % 8)```: se hace un desplazamiento a la izquierda el resultado de calcular el modulo de un byte de ```key``` (key_byte) por 8
- ```(c >> (8 - key_byte % 8)))```: Se hace un desplazamiento a la derecha del resultado de ```(8 - key_byte % 8)```, que indica cuandos bits se deben de desplazar a la derecha
para que cubran los 8 bits de un byte

```c
c = ((c << (key_byte % 8)) | (c >> (8 - key_byte % 8))) & 0xFF
```

Y para finalizar se asigna el valor del caracter cifrado de la posicion correspondiente en ```datos_cifrados```

```c
datos_decifrados[i] = c;
```

## Uso

**Instalar**

```sudo make install```

**Desinstalar**

```sudo make unistall```

**Clean**

```sudo make clean```


---

```c
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "liska.h"


int main() {
    const char *texto = "Wired";
    const char *key = "Lain";
    char *datos_cifrados = (char*)malloc(strlen(texto) + 1);
    char *datos_decifrados = (char*)malloc(strlen(texto) + 1);

    cifrar_liska(texto, key, datos_cifrados);
    printf("Cifrado: ");
    hexadecimal(datos_cifrados);

    decifrar_liska(datos_cifrados, key, datos_decifrados);
    printf("Descifrado: %s\n", datos_decifrados);

    return 0;
}
```

Tambien puedes optar sin usar malloc

```c
#include <string.h>
#include <stdio.h>
#include "liska.h"

int main() {
    const char texto[] = "Wired";
    const char key[] = "Lain";
    
    char datos_cifrados[strlen(texto) + 1];
    char datos_decifrados[strlen(texto) + 1];

    cifrar_liska(texto, key, datos_cifrados);
    printf("Cifrado: ");
    hexadecimal(datos_cifrados);

    decifrar_liska(datos_cifrados, key, datos_decifrados);
    printf("Descifrado: %s\n", datos_decifrados);

    return 0;
}
```

**Puntos adicionales**

- En todo el codigo veras que existen valores contantes los que estan en la funcion ```split_mix```, estos son valores de la implementacion original de los algoritmos que se estan usando de acuerdo a wiki, sin embargo, puedes modificar esos valores (no lo recomiendo), de hecho, cambiarias por completo el resultado del cifrado modificando aun que sea solo un byte

