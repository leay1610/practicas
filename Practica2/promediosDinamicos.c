#include <stdio.h>
#include <stdlib.h>

void main() {
    printf("=== Calculadora de Promedio Dinámica ===\n\n");
    int opcion = 0;

    // variables dinámicas
    double *calificaciones = NULL;
    int total = 0; // cantidad total de calificaciones guardadas

    while (opcion != 2) {
        printf("1) Ingresar calificaciones\n");
        printf("2) Salir\n");
        printf("Seleccione una opcion: ");
        scanf("%i", &opcion);

        switch (opcion) {
        case 1: {
            int n;
            printf("\n¿Cuántas calificaciones desea ingresar? ");
            scanf("%d", &n);

            // Reservar o ampliar memoria
            if (total == 0) {
                calificaciones = (double *)malloc(n, sizeof(double));
                if (calificaciones == NULL) {
                    printf("No se pudo asignar memoria.\n");
                    return;
                }
            } else {
                double *tmp = (double *)realloc(calificaciones, (total + n) * sizeof(double));
                if (tmp == NULL) {
                    printf("Error al reasignar memoria.\n");
                    free(calificaciones);
                    return;
                }
                calificaciones = tmp;
            }

            // Ingresar calificaciones
            for (int i = 0; i < n; i++) {
                printf("Ingrese la calificación %d: ", total + i + 1);
                scanf("%lf", (calificaciones+ total + i));
            }

            total += n;

            // Calcular promedio
            double suma = 0;
            double *ptr=calificaciones;
            for (int i = 0; i < total; i++) {
                suma += *(ptr + i);
            }
            double promedio = suma / total;

            printf("\nPromedio actual: %.2f\n", promedio);
            if (promedio >= 7)
                printf("Estado: APROBADO\n");
            else
                printf("Estado: REPROBADO\n");

            break;
        }

        case 2:
            // se libera la memoria reservada
            free(calificaciones);
            calificaciones = NULL;

            // mensaje de despedida
            printf("\nMemoria liberada!!! Hasta luego.\n");
            break;

        default:
            // opción del usuario inválida
            printf("\n=== Opcion invalida === \nPor favor seleccione una opcion del menu:\n");
        }
    }
}
