#include <stdio.h>

typedef struct {
    char nombre[50];
    int matricula;
    double calificaciones[4];
    double promedio;
} Estudiante;

// AGREGAR: Prototipos de funciones
void capturarEstudiante(Estudiante *est);
void calcularPromedio(Estudiante *est);
void mostrarEstudiante(Estudiante est);

double promedioGrupal(Estudiante grupo[], int tam); 
void estudiantesMayores8(Estudiante grupo[], int tam);
int buscarPorMatricula(Estudiante grupo[], int tam, int matricula); 

int main(){
    Estudiante grupo[4];
    
    printf("=== SISTEMA DE GESTIÓN ACADÉMICA ===\n");
    
    //Capturar datos de 4 estudiantes
    for(int i = 0; i < 4; i++) {
        printf("\nEstudiante %d:\n", i+1);
        capturarEstudiante(&grupo[i]);
    }
    
    // Mostrar todos los estudiantes
    printf("\n=== LISTA COMPLETA DE ESTUDIANTES ===\n");
    //Mostrar información de todos los estudiantes
    for(int i = 0; i < 4; i++) {
        mostrarEstudiante(grupo[i]);
    }
    double promGral = promedioGrupal(grupo, 4);
    printf("\n=== PROMEDIO GENERAL DEL GRUPO ===\n");
    printf("El promedio general del grupo es: %.2f\n", promGral);
    
    printf("\n=== ESTUDIANTES CON PROMEDIO MAYOR A 8.0 ===\n");
    estudiantesMayores8(grupo, 4);

    int matBusqueda;
    printf("\n=== BÚSQUEDA DE ESTUDIANTE POR MATRÍCULA ===\n");
    printf("Ingrese la matrícula a buscar: ");
    scanf("%d", &matBusqueda);
    int indice = buscarPorMatricula(grupo, 4, matBusqueda);

    if (indice != -1) {
        printf("Estudiante encontrado en el índice: %d\n", indice);
        mostrarEstudiante(grupo[indice]);
    } else {
        printf("Estudiante no encontrado. El valor devuelto es: %d\n", indice);
    }

    //mejor promedio
    int mejorIndex = 0;
    for(int i = 1; i < 4; i++) {
        if(grupo[i].promedio > grupo[mejorIndex].promedio) {
            mejorIndex = i;
        }
    }
    
    printf("\n=== ESTUDIANTE CON MEJOR PROMEDIO ===\n");
    printf("Nombre: %s\n", grupo[mejorIndex].nombre);
    printf("Promedio: %.2f\n", grupo[mejorIndex].promedio);
    
    return 0;
}

// Funciones de captura, cálculo y muestra
void capturarEstudiante(Estudiante *est) {
    printf("Ingrese nombre: ");
    scanf("%s", est->nombre);
    
    printf("Ingrese matrícula: ");
    scanf("%d", &est->matricula);
    
    for(int i = 0; i < 4; i++) {
        printf("Ingrese calificación %d: ", i+1);
        scanf("%lf", &est->calificaciones[i]);
    }
    
    calcularPromedio(est);
}

void calcularPromedio(Estudiante *est) {
    double suma = 0;
    for(int i = 0; i < 4; i++) {
        suma += est->calificaciones[i];
    }
    est->promedio = suma / 4;
}

void mostrarEstudiante(Estudiante est) {
    printf("\n--- Información del Estudiante ---\n");
    printf("Nombre: %s\n", est.nombre);
    printf("Matrícula: %d\n", est.matricula);
    printf("Calificaciones: ");
    for(int i = 0; i < 4; i++) {
        printf("%.2f ", est.calificaciones[i]);
    }
    printf("\nPromedio: %.2f\n", est.promedio);
}
double promedioGrupal(Estudiante grupo[], int tam) {
    double sumaTotal = 0.0;
    for (int i = 0; i < tam; i++) {
        sumaTotal += grupo[i].promedio;
    }
    if (tam > 0) {
        return sumaTotal / tam;
    } else {
        return 0.0;
    }
}
void estudiantesMayores8(Estudiante grupo[], int tam) {
    int cont = 0;
    for (int i = 0; i < tam; i++) {
        if (grupo[i].promedio > 8.0) {
            mostrarEstudiante(grupo[i]);
            cont++;
        }
    }
    if (cont == 0) {
        printf("No hay estudiantes con promedio mayor a 8.0.\n");
    }
}
int buscarPorMatricula(Estudiante grupo[], int tam, int matricula) {
    for (int i = 0; i < tam; i++) {
        if (grupo[i].matricula == matricula) {
            return i; // Regresa el índice del arreglo
        }
    }
    return -1; // Regresa -1 si no se encuentra
}
