#include <stdio.h>
#include <string.h>

#define N 4  // número de estudiantes (según instrucciones: arreglo de 4 estructuras)

typedef struct {
    char nombre[50];
    int matricula;
    double calificaciones[4];
    double promedio;
} Estudiante;

/* Prototipos */
void capturarEstudiante(Estudiante *est);
void calcularPromedio(Estudiante *est);
void mostrarEstudiante(Estudiante est);

int main() {
    Estudiante grupo[N];

    printf("=== SISTEMA DE GESTIÓN ACADÉMICA ===\n");

    // Capturar datos de N estudiantes
    for (int i = 0; i < N; i++) {
        printf("\nEstudiante %d:\n", i + 1);
        capturarEstudiante(&grupo[i]);
    }

    // Mostrar todos los estudiantes
    printf("\n=== LISTA COMPLETA DE ESTUDIANTES ===\n");
    for (int i = 0; i < N; i++) {
        mostrarEstudiante(grupo[i]);
    }

    // Encontrar el mejor promedio
    int mejorIndex = 0;
    for (int i = 1; i < N; i++) {
        if (grupo[i].promedio > grupo[mejorIndex].promedio) {
            mejorIndex = i;
        }
    }

    printf("\n=== ESTUDIANTE CON MEJOR PROMEDIO ===\n");
    printf("Nombre: %s\n", grupo[mejorIndex].nombre);
    printf("Matrícula: %d\n", grupo[mejorIndex].matricula);
    printf("Promedio: %.2f\n", grupo[mejorIndex].promedio);

    return 0;
}

void capturarEstudiante(Estudiante *est) {
    // Limpiar el buffer de entrada antes de usar fgets si viene de un scanf anterior
    // (En este programa, usamos scanf para números; por seguridad, dejamos aquí)
    int c;
    while ((c = getchar()) != '\n' && c != EOF) { }

    printf("Ingrese nombre: ");
    if (fgets(est->nombre, sizeof(est->nombre), stdin) != NULL) {
        // quitar '\n' final si existe
        size_t len = strlen(est->nombre);
        if (len > 0 && est->nombre[len - 1] == '\n') {
            est->nombre[len - 1] = '\0';
        }
    } else {
        // si falla fgets, dejar nombre vacío
        est->nombre[0] = '\0';
    }

    printf("Ingrese matrícula: ");
    while (scanf("%d", &est->matricula) != 1) {
        // manejo simple de error: limpiar entrada y pedir de nuevo
        printf("Entrada inválida. Ingrese matrícula (número entero): ");
        while ((c = getchar()) != '\n' && c != EOF) { }
    }

    // Capturar 4 calificaciones (double)
    for (int i = 0; i < 4; i++) {
        printf("Ingrese calificación %d: ", i + 1);
        while (scanf("%lf", &est->calificaciones[i]) != 1) {
            printf("Entrada inválida. Ingrese una calificación (número): ");
            while ((c = getchar()) != '\n' && c != EOF) { }
        }
    }

    // Calcular y guardar promedio
    calcularPromedio(est);
}

void calcularPromedio(Estudiante *est) {
    double suma = 0.0;
    for (int i = 0; i < 4; i++) {
        suma += est->calificaciones[i];
    }
    est->promedio = suma / 4.0;
}

void mostrarEstudiante(Estudiante est) {
    printf("\n--- Información del Estudiante ---\n");
    printf("Nombre: %s\n", est.nombre);
    printf("Matrícula: %d\n", est.matricula);
    for (int i = 0; i < 4; i++) {
        printf("Calificación %d: %.2f\n", i + 1, est.calificaciones[i]);
    }
    printf("Promedio: %.2f\n", est.promedio);
}
