#ifndef FUNCIONES_H
#define FUNCIONES_H

// Cabecera para practica1 - Prototipos y tipos
typedef struct {
    int x;
    int y;
} Punto;

void swap(int *a, int *b);
int longitudCadena(char *cadena);
void invertirArreglo(int *arr, int size);
int cuentaPares(int *arr, int size);

#endif // FUNCIONES_H

