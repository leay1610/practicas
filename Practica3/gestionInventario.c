#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Variables globales del inventario (inicializadas a NULL / 0) */
char **nombresProductos = NULL; /* puntero doble para nombres */
int *cantidades = NULL;         /* puntero para cantidades */
double *precios = NULL;         /* puntero para precios */
int numProductos = 0;           /* número actual de productos */

/* Prototipos */
int agregarProductoInteractive(); /* agrega pidiendo datos al usuario */
void mostrarProductos();
double calcularValorTotalInventario();
void liberarInventario();
void cargarInventarioPorDefecto(); /* carga los datos de la tabla (no pide al usuario) */

int main() {
    int opcion;

    printf("=== SISTEMA DE INVENTARIO ===\n");

    /* Cargar inventario inicial con los datos de la práctica (tabla). */
    cargarInventarioPorDefecto();

    /* Menú interactivo */
    do {
        printf("\n--- MENU PRINCIPAL ---\n");
        printf("1. Agregar producto\n");
        printf("2. Mostrar inventario\n");
        printf("3. Calcular valor total del inventario\n");
        printf("4. Salir\n");
        printf("Seleccione opcion: ");
        if (scanf("%d", &opcion) != 1) {
            /* Entrada inválida: limpiar buffer y forzar valor inválido */
            int c;
            while ((c = getchar()) != '\n' && c != EOF) {}
            opcion = -1;
        }

        switch(opcion) {
            case 1:
                if (agregarProductoInteractive()) {
                    printf("Producto agregado correctamente.\n");
                } else {
                    printf("Error al agregar producto.\n");
                }
                break;
            case 2:
                mostrarProductos();
                break;
            case 3: {
                double total = calcularValorTotalInventario();
                printf("\nValor total inventario: $%.2f\n", total);
                break;
            }
            case 4:
                printf("Saliendo del sistema...\n");
                break;
            default:
                printf("Opcion invalida.\n");
        }
    } while(opcion != 4);

    /* Liberar memoria antes de salir */
    liberarInventario();
    return 0;
}

/* Agregar producto pidiendo datos al usuario.
   Devuelve 1 si fue exitoso, 0 si hubo error.
   NOTA: usamos exclusivamente notación de punteros para acceder a los arreglos. */
int agregarProductoInteractive() {
    char buffer[256];
    char *nombreNuevo = NULL;
    int cantidadNueva;
    double precioNuevo;

    /* Limpiar entrada pendiente antes de usar fgets (por si scanf dejó '\n') */
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {}

    printf("\n=== Agregar Producto ===\n");
    printf("Nombre: ");
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
        return 0;
    }
    /* quitar salto de línea */
    size_t len = strlen(buffer);
    if (len > 0 && buffer[len-1] == '\n') buffer[len-1] = '\0';

    printf("Cantidad: ");
    if (scanf("%d", &cantidadNueva) != 1) return 0;
    printf("Precio: ");
    if (scanf("%lf", &precioNuevo) != 1) return 0;

    /* 1) Realloc para nombresProductos (puntero doble) */
    char **tmpNombres = (char**) realloc(nombresProductos, (numProductos + 1) * sizeof(char*));
    if (tmpNombres == NULL) {
        /* error: no se pudo realocar */
        return 0;
    }
    nombresProductos = tmpNombres;

    /* 2) Reservar espacio exacto para el nombre (strlen + 1) */
    nombreNuevo = (char*) malloc((strlen(buffer) + 1) * sizeof(char));
    if (nombreNuevo == NULL) {
        /* en caso de fallo, revertir realloc hecho arriba */
        return 0;
    }
    strcpy(nombreNuevo, buffer); /* copiar nombre */

    /* 3) Realloc para cantidades */
    int *tmpCant = (int*) realloc(cantidades, (numProductos + 1) * sizeof(int));
    if (tmpCant == NULL) {
        free(nombreNuevo);
        return 0;
    }
    cantidades = tmpCant;

    /* 4) Realloc para precios */
    double *tmpPrecios = (double*) realloc(precios, (numProductos + 1) * sizeof(double));
    if (tmpPrecios == NULL) {
        /* revertir lo necesario */
        free(nombreNuevo);
        return 0;
    }
    precios = tmpPrecios;

    /* 5) Asignar valores en la "última" posición usando notación de punteros */
    *(nombresProductos + numProductos) = nombreNuevo;
    *(cantidades + numProductos) = cantidadNueva;
    *(precios + numProductos) = precioNuevo;

    numProductos++; /* aumentamos el contador */
    return 1;
}

/* Mostrar todos los productos */
void mostrarProductos() {
    printf("\n=== Productos %d ===\n", numProductos);
    int i;
    for (i = 0; i < numProductos; i++) {
        char *nombre = *(nombresProductos + i);
        int cant = *(cantidades + i);
        double prec = *(precios + i);
        double valor = cant * prec;
        printf("Pos %d: %s - Cant: %d - Precio: $%.2f - Valor: $%.2f\n", i, nombre, cant, prec, valor);
    }
}

/* Calcular valor total del inventario */
double calcularValorTotalInventario() {
    double total = 0.0;
    int i;
    for (i = 0; i < numProductos; i++) {
        int cant = *(cantidades + i);
        double prec = *(precios + i);
        total += (double)cant * prec;
    }
    return total;
}

/* Liberar toda la memoria usada por el inventario */
void liberarInventario() {
    int i;
    /* Primero liberar cada nombre individual (nivel interno del puntero doble) */
    for (i = 0; i < numProductos; i++) {
        char *p = *(nombresProductos + i);
        if (p != NULL) {
            free(p);
            /* opcion: *(nombresProductos + i) = NULL; */
        }
    }

    /* Luego liberar los arrays principales (punteros) */
    if (nombresProductos != NULL) {
        free(nombresProductos);
        nombresProductos = NULL;
    }
    if (cantidades != NULL) {
        free(cantidades);
        cantidades = NULL;
    }
    if (precios != NULL) {
        free(precios);
        precios = NULL;
    }
    numProductos = 0;
}

/* Carga los productos por defecto indicados en la práctica (Tabla en el PDF) */
void cargarInventarioPorDefecto() {
    /* Datos tomados de la tabla proporcionada en la práctica (pos 0..8). */
    /* Ver PDF: Tabla de Datos de la práctica. */
    const char *nombres[] = {
        "Laptop HP",
        "Mouse Inalambrico",
        "Teclado Mecanico",
        "Camisa Casual",
        "Jeans",
        "Zapatos Deportivos",
        "Arroz",
        "Frijol",
        "Aceite"
    };
    const int cants[] = {5, 15, 8, 20, 12, 10, 50, 40, 30};
    const double preciosDef[] = {1200.00, 25.50, 75.00, 35.00, 45.00, 60.00, 2.50, 3.00, 4.50};
    int i;

    for (i = 0; i < 9; i++) {
        /* Para no pedir al usuario, replicamos la lógica de asignación
           similar a agregarProductoInteractive pero sin pedir entrada. */

        /* Realloc nombresProductos */
        char **tmpNombres = (char**) realloc(nombresProductos, (numProductos + 1) * sizeof(char*));
        if (tmpNombres == NULL) {
            printf("Error al reservar memoria inicial (nombres).\n");
            return;
        }
        nombresProductos = tmpNombres;

        /* Asignar espacio exacto y copiar el nombre */
        char *nombreNuevo = (char*) malloc((strlen(nombres[i]) + 1) * sizeof(char));
        if (nombreNuevo == NULL) {
            printf("Error al reservar memoria para nombre %s\n", nombres[i]);
            return;
        }
        strcpy(nombreNuevo, nombres[i]);

        /* Realloc cantidades */
        int *tmpCant = (int*) realloc(cantidades, (numProductos + 1) * sizeof(int));
        if (tmpCant == NULL) {
            free(nombreNuevo);
            printf("Error al reservar memoria inicial (cantidades).\n");
            return;
        }
        cantidades = tmpCant;

        /* Realloc precios */
        double *tmpPrecios = (double*) realloc(precios, (numProductos + 1) * sizeof(double));
        if (tmpPrecios == NULL) {
            free(nombreNuevo);
            printf("Error al reservar memoria inicial (precios).\n");
            return;
        }
        precios = tmpPrecios;

        /* Asignar en la posición (usando notación de punteros) */
        *(nombresProductos + numProductos) = nombreNuevo;
        *(cantidades + numProductos) = cants[i];
        *(precios + numProductos) = preciosDef[i];

        numProductos++;
    }
}
