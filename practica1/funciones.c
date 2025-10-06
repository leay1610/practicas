#include <stdio.h>
#include "funciones.h"

/*
 Implementacion de funciones para la practica 1.
 Autor: Christian Yael Correa Tellez
 Profesor: Ernesto Del Moral Arcos
*/

// swap: intercambia los valores apuntados por a y b.
// parametros:
//   - int *a: puntero al primer entero
//   - int *b: puntero al segundo entero
void swap(int *a, int *b) {
    if (a == NULL || b == NULL) return;
    int temp = *a;
    *a = *b;
    *b = temp;
}
// longitudCadena: retorna la longitud de la cadena sin usar strlen.
// parametros:
//   - char *cadena: puntero a la cadena terminada en '\\0'
// retorno: int con la longitud (0 si cadena es NULL)
int longitudCadena(char *cadena) {
    if (cadena == NULL) return 0;
    int contador = 0;
    while (*cadena != '\0') {
        contador++;
        cadena++;
    }
    return contador;
}
