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
// invertirArreglo: invierte un arreglo de enteros in-place usando apuntadores.
// parametros:
//   - int *arr: puntero al primer elemento del arreglo
//   - int size: tamano del arreglo
void invertirArreglo(int *arr, int size) {
    if (arr == NULL || size <= 0) return;
    int *inicio = arr;
    int *fin = arr + size - 1;
    while (inicio < fin) {
        int temp = *inicio;
        *inicio = *fin;
        *fin = temp;
        inicio++;
        fin--;
    }
}
// cuentaPares: cuenta la cantidad de numeros pares en un arreglo.
// parametros:
//   - int *arr: puntero al arreglo
//   - int size: tamano del arreglo
// retorno: int con la cantidad de pares (0 si arr es NULL o size <= 0)
int cuentaPares(int *arr, int size) {
    if (arr == NULL || size <= 0) return 0;
    int contador = 0;
    for (int i = 0; i < size; i++) {
        if (*(arr + i) % 2 == 0) contador++;
    }
    return contador;
}
