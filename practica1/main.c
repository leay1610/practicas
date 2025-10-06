#include <stdio.h>
#include "funciones.h"

int main(void) {
    // Prueba swap
    int a = 5, b = 10;
    printf("Antes swap: a=%d, b=%d\n", a, b);
    swap(&a, &b);
    printf("Despues swap: a=%d, b=%d\n\n", a, b);

    // Prueba longitudCadena
    char texto[] = "Hola mundo";
    printf("Cadena: \"%s\" -> longitud: %d\n", texto, longitudCadena(texto));
    printf("Cadena NULL -> longitud: %d\n\n", longitudCadena(NULL));

    // Prueba invertirArreglo y cuentaPares
    int numeros[] = {1, 2, 3, 4, 5, 6};
    int size = sizeof(numeros) / sizeof(numeros[0]);
    printf("Arreglo original: ");
    for (int i = 0; i < size; i++) printf("%d ", numeros[i]);
    printf("\n");

    invertirArreglo(numeros, size);
    printf("Arreglo invertido: ");
    for (int i = 0; i < size; i++) printf("%d ", numeros[i]);
    printf("\n");

    printf("Pares en arreglo: %d\n", cuentaPares(numeros, size));
    return 0;
}
