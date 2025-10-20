/* promDinamica - Practica 2: Memoria dinamica en C
 * Archivo: Practica2/promediosDinamicos.c
 *
 * Compilar:
 * gcc -std=c11 -Wall -Wextra -o prom promediosDinamicos.c
 *
 * Ejecutar:
 * ./prom
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>

#define BUFFER_SIZE 128

/* Lee una línea y la deja en buffer (sin '\n'). */
static void read_line(char *buffer, size_t size) {
    if (fgets(buffer, (int)size, stdin) == NULL) {
        buffer[0] = '\0';
        return;
    }
    size_t len = strlen(buffer);
    if (len > 0 && buffer[len-1] == '\n') buffer[len-1] = '\0';
}

/* Lee entero > 0 con prompt y validación */
static int read_positive_int(const char *prompt) {
    char buf[BUFFER_SIZE];
    long val;
    char *endptr;
    while (1) {
        printf("%s", prompt);
        read_line(buf, sizeof(buf));
        errno = 0;
        val = strtol(buf, &endptr, 10);
        if (errno == 0 && endptr != buf) {
            if (val > 0) return (int)val;
        }
        printf("Entrada no valida. Introduce un entero mayor que 0.\n");
    }
}

/* Lee double en rango [min, max] */
static double read_double_range(const char *prompt, double min, double max) {
    char buf[BUFFER_SIZE];
    double val;
    char *endptr;
    while (1) {
        printf("%s", prompt);
        read_line(buf, sizeof(buf));
        errno = 0;
        val = strtod(buf, &endptr);
        if (errno == 0 && endptr != buf) {
            if (val >= min && val <= max) return val;
        }
        printf("Valor no valido. Debe ser un numero entre %.2f y %.2f.\n", min, max);
    }
}

/* Lee respuesta 's' o 'n' (minúscula o mayúscula aceptada) */
static char read_s_n(const char *prompt) {
    char buf[BUFFER_SIZE];
    while (1) {
        printf("%s", prompt);
        read_line(buf, sizeof(buf));
        if (buf[0] != '\0') {
            char c = (char)tolower((unsigned char)buf[0]);
            if (c == 's' || c == 'n') return c;
        }
        printf("Respuesta no valida. Ingresa 's' o 'n'.\n");
    }
}

int main(void) {
    double *calificaciones = NULL;
    int total = 0;

    while (1) {
        printf("\n=== Calculadora de Promedio Dinamica ===\n");
        printf("1) Ingresar calificaciones\n");
        printf("2) Salir\n");
        int opcion = read_positive_int("Seleccione una opcion: ");
        if (opcion == 2) {
            break;
        } else if (opcion == 1) {
            int n = read_positive_int("¿Cuantas calificaciones desea ingresar? ");
            /* Primera vez: usar calloc; si ya hay calificaciones, usar realloc */
            if (total == 0) {
                /* calloc para inicializar a 0 (buenas practicas) */
                calificaciones = (double *) calloc((size_t)n, sizeof(double));
                if (calificaciones == NULL) {
                    perror("No se pudo asignar memoria");
                    return EXIT_FAILURE;
                }
            } else {
                /* ampliar con realloc: obligatorio segun enunciado */
                double *tmp = (double *) realloc(calificaciones, (size_t)(total + n) * sizeof(double));
                if (tmp == NULL) {
                    perror("No se pudo reasignar memoria");
                    /* no liberamos calificaciones porque tmp==NULL y calificaciones sigue valido */
                    free(calificaciones);
                    return EXIT_FAILURE;
                }
                calificaciones = tmp;
            }

            /* Capturar nuevas calificaciones */
            for (int i = 0; i < n; ++i) {
                int idx = total + i + 1;
                char prompt[80];
                snprintf(prompt, sizeof(prompt), "Ingrese la calificacion %d: ", idx);
                calificaciones[total + i] = read_double_range(prompt, 0.0, 10.0);
            }
            total += n;

            /* Calcular promedio acumulado */
            double suma = 0.0;
            for (int i = 0; i < total; ++i) suma += calificaciones[i];
            double promedio = suma / (double) total;
            printf("\nPromedio: %.2f\n", promedio);
            printf("Estado: %s\n", (promedio >= 7.0) ? "APROBADO" : "REPROBADO");

            /* Preguntar si desea agregar mas */
            char resp = read_s_n("¿Desea agregar mas calificaciones? (s/n) ");
            if (resp == 's') {
                continue;
            } else {
                continue;
            }

        } else {
            printf("Opcion invalida. Debe ser 1 o 2.\n");
        }
    }

    /* Al salir, liberar memoria */
    free(calificaciones);
    calificaciones = NULL;
    printf("¡Memoria liberada! Hasta luego.\n");
    return 0;
}
