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
