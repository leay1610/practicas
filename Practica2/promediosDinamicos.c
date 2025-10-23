#include <stdio.h>
#include <stdlib.h>

int main() {
    printf("=== Calculadora de Promedio Dinámica ===\n\n");

    int opcion = 0;
    double *calificaciones = NULL;
    int total = 0; // total de calificaciones almacenadas

    do {
        printf("\n1) Ingresar calificaciones\n");
        printf("2) Salir\n");
        printf("Seleccione una opción: ");
        scanf("%d", &opcion);

        switch (opcion) {
        case 1: {
            int continuar = 1; // control de agregar más calificaciones (1=sí, 2=no)

            do {
                int n;
                printf("\n¿Cuántas calificaciones desea ingresar? ");
                scanf("%d", &n);

                // Asignar o ampliar memoria dinámicamente
                if (total == 0) {
                    calificaciones = (double *)malloc(n * sizeof(double));
                    if (calificaciones == NULL) {
                        printf("Error: no se pudo asignar memoria.\n");
                        return 1;
                    }
                } else {
                    double *tmp = (double *)realloc(calificaciones, (total + n) * sizeof(double));
                    if (tmp == NULL) {
                        printf("Error: no se pudo reasignar memoria.\n");
                        free(calificaciones);
                        return 1;
                    }
                    calificaciones = tmp;
                }

                // Ingresar calificaciones validadas (0 a 10)
                for (int i = 0; i < n; i++) {
                    double nota;
                    do {
                        printf("Ingrese la calificación %d: ", total + i + 1);
                        scanf("%lf", &nota);
                        if (nota < 0 || nota > 10) {
                            printf("Error: la calificación debe estar entre 0 y 10.\n");
                        }
                    } while (nota < 0 || nota > 10);

                    *(calificaciones + total + i) = nota;
                }

                total += n;

                // Calcular el promedio
                double suma = 0;
                for (int i = 0; i < total; i++) {
                    suma += *(calificaciones + i);
                }

                double promedio = suma / total;
                printf("\nPromedio actual: %.2f\n", promedio);
                printf("Estado: %s\n", (promedio >= 7.0) ? "APROBADO" : "REPROBADO");
                printf("Total de calificaciones registradas: %d\n", total);

                // Preguntar si desea agregar más calificaciones (con menú 1/2)
                do {
                    printf("\n¿Desea agregar más calificaciones?\n");
                    printf("1) Sí\n");
                    printf("2) No\n");
                    printf("Seleccione una opción: ");
                    scanf("%d", &continuar);

                    if (continuar != 1 && continuar != 2) {
                        printf("Opción inválida. Ingrese 1 o 2.\n");
                    }
                } while (continuar != 1 && continuar != 2);

            } while (continuar == 1); // si elige 1, vuelve a pedir calificaciones

            break;
        }

        case 2:
            if (calificaciones != NULL) {
                free(calificaciones);
                calificaciones = NULL;
            }
            printf("\n¡Memoria liberada! Hasta luego.\n");
            break;

        default:
            printf("\n=== Opción inválida ===\nPor favor seleccione una opción válida.\n");
        }

    } while (opcion != 2);

    return 0;
}
