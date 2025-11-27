/* biblioteca.c
   Proyecto: Sistema de gestión para biblioteca universitaria
   Materia: Introducción a la Ingeniería del Software - UACM
   Profesor: Ernesto Del Moral Arcos

   Notas:
   - Usa arreglos dinámicos (punteros dobles) para libros, usuarios y préstamos.
   - Implementa todas las funciones solicitadas en el enunciado.
   - Incluye datos de prueba obligatorios en la función cargarDatosPrueba().
   - Comentarios explicativos incluidos.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/* ---------- Definiciones de estructuras ---------- */

/* Estructura Libro */
typedef struct {
    int id;                     // Identificador autoincremental
    char titulo[101];           // título (hasta 100 chars + '\0')
    char autor[51];             // autor (hasta 50)
    char isbn[21];              // ISBN (hasta 20)
    int anio;                   // año de publicación
    char categoria[31];         // categoría (hasta 30)
    int disponible;             // 1 disponible, 0 prestado
} Libro;

/* Estructura Usuario */
typedef struct {
    int id;                     // id usuario
    char nombre[51];
    char carrera[51];
    int matricula;
    int activo;                 // 1 activo, 0 inactivo
} Usuario;

/* Estructura Prestamo */
typedef struct {
    int id;                     // id préstamo
    int id_libro;
    int id_usuario;
    char fecha_prestamo[20];    // "DD/MM/AAAA"
    char fecha_devolucion[20];  // fecha límite "DD/MM/AAAA"
    char fecha_devolucion_real[20]; // fecha real de devolución ("" si no devuelto)
    int devuelto;               // 1 = devuelto, 0 = activo
} Prestamo;

/* ---------- Variables globales de gestión ---------- */
Libro **libros = NULL;
Usuario **usuarios = NULL;
Prestamo **prestamos = NULL;

int totalLibros = 0;
int totalUsuarios = 0;
int totalPrestamos = 0;

int ultimoIdLibro = 0;
int ultimoIdUsuario = 0;
int ultimoIdPrestamo = 0;

/* ---------- Funciones auxiliares ---------- */

/* Generar ID autoincremental */
int generarId(int *ultimo_id) {
    (*ultimo_id)++;
    return *ultimo_id;
}

/* Obtener fecha actual en formato DD/MM/AAAA */
void obtenerFechaActual(char *buffer, size_t bufsize) {
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    snprintf(buffer, bufsize, "%02d/%02d/%04d", tm.tm_mday, tm.tm_mon + 1, tm.tm_year + 1900);
}

/* Comprueba si string es dígitos (útil en validación) */
int esNumero(const char *s) {
    if (s == NULL || *s == '\0') return 0;
    for (int i = 0; s[i]; i++) if (s[i] < '0' || s[i] > '9') return 0;
    return 1;
}

/* Validar fecha en formato DD/MM/AAAA y rangos */
int validaFecha(const char *fecha) {
    if (fecha == NULL) return 0;
    int d, m, a;
    if (sscanf(fecha, "%d/%d/%d", &d, &m, &a) != 3) return 0;
    if (a < 1 || m < 1 || m > 12 || d < 1) return 0;
    int diasMes[] = {0,31,28,31,30,31,30,31,31,30,31,30,31};
    int esBisiesto = ( (a%4==0 && a%100!=0) || (a%400==0) );
    if (esBisiesto) diasMes[2] = 29;
    if (d > diasMes[m]) return 0;
    return 1;
}

/* Convierte fecha DD/MM/AAAA a estructura tm (simple) */
void fechaToDMY(const char *fecha, int *d, int *m, int *y) {
    *d = *m = *y = 0;
    sscanf(fecha, "%d/%d/%d", d, m, y);
}

/* Calcular fecha_devolucion = fecha_prestamo + 15 días (considerando meses y bisiestos) */
void calcularFechaDevolucion(const char *fecha_prestamo, char *fecha_devolucion) {
    int d, m, y;
    fechaToDMY(fecha_prestamo, &d, &m, &y);
    int diasMes[] = {0,31,28,31,30,31,30,31,31,30,31,30,31};
    int esBisiesto = ( (y%4==0 && y%100!=0) || (y%400==0) );
    if (esBisiesto) diasMes[2] = 29;
    int add = 15;
    d += add;
    while (1) {
        esBisiesto = ( (y%4==0 && y%100!=0) || (y%400==0) );
        diasMes[2] = esBisiesto ? 29 : 28;
        if (d <= diasMes[m]) break;
        d -= diasMes[m];
        m++;
        if (m > 12) { m = 1; y++; }
    }
    snprintf(fecha_devolucion, 20, "%02d/%02d/%04d", d, m, y);
}

/* Comparar fechas en formato DD/MM/AAAA
   retorna: -1 si f1 < f2, 0 si iguales, 1 si f1 > f2 */
int compararFechas(const char *f1, const char *f2) {
    int d1,m1,y1, d2,m2,y2;
    fechaToDMY(f1,&d1,&m1,&y1);
    fechaToDMY(f2,&d2,&m2,&y2);
    if (y1 < y2) return -1;
    if (y1 > y2) return 1;
    if (m1 < m2) return -1;
    if (m1 > m2) return 1;
    if (d1 < d2) return -1;
    if (d1 > d2) return 1;
    return 0;
}

/* ---------- Gestión de Libros ---------- */

/* Buscar índice de libro por ID, retorna -1 si no existe */
int indexLibroPorId(Libro **librosArr, int tot, int id) {
    for (int i = 0; i < tot; i++) if (librosArr[i]->id == id) return i;
    return -1;
}

/* Buscar libro por título (búsqueda parcial, case-insensitive), retorna puntero o NULL */
Libro* buscarLibroPorTitulo(Libro **librosArr, int totLibros, const char *titulo) {
    if (!titulo) return NULL;
    for (int i = 0; i < totLibros; i++) {
        if (strcasestr(librosArr[i]->titulo, titulo) != NULL) return librosArr[i];
    }
    return NULL;
}

/* Validar ISBN duplicado, retorna 1 si existe, 0 si no */
int existeISBN(Libro **librosArr, int tot, const char *isbn) {
    for (int i = 0; i < tot; i++) {
        if (strcmp(librosArr[i]->isbn, isbn) == 0) return 1;
    }
    return 0;
}

/* Agregar libro (interactivo). Retorna el nuevo puntero al array (libros) */
Libro** agregarLibro(Libro **librosArr, int *totLibros) {
    Libro *nuevo = (Libro*) malloc(sizeof(Libro));
    if (!nuevo) { printf("Error: memoria insuficiente.\n"); return librosArr; }

    printf("Agregar nuevo libro:\n");
    printf("Titulo: ");
    getchar(); // consumir \n anterior si existe
    fgets(nuevo->titulo, sizeof(nuevo->titulo), stdin);
    nuevo->titulo[strcspn(nuevo->titulo, "\n")] = 0;

    printf("Autor: ");
    fgets(nuevo->autor, sizeof(nuevo->autor), stdin);
    nuevo->autor[strcspn(nuevo->autor, "\n")] = 0;

    printf("ISBN: ");
    fgets(nuevo->isbn, sizeof(nuevo->isbn), stdin);
    nuevo->isbn[strcspn(nuevo->isbn, "\n")] = 0;

    if (existeISBN(librosArr, *totLibros, nuevo->isbn)) {
        printf("Error: ISBN duplicado. Operación cancelada.\n");
        free(nuevo);
        return librosArr;
    }

    char anioStr[10];
    printf("Año de publicación: ");
    fgets(anioStr, sizeof(anioStr), stdin);
    nuevo->anio = atoi(anioStr);

    printf("Categoria: ");
    fgets(nuevo->categoria, sizeof(nuevo->categoria), stdin);
    nuevo->categoria[strcspn(nuevo->categoria, "\n")] = 0;

    nuevo->disponible = 1;
    nuevo->id = generarId(&ultimoIdLibro);

    /* redimensionar arreglo de punteros */
    Libro **temp = (Libro**) realloc(librosArr, sizeof(Libro*) * ((*totLibros) + 1));
    if (!temp) {
        printf("Error: realloc falló.\n");
        free(nuevo);
        return librosArr;
    }
    librosArr = temp;
    librosArr[*totLibros] = nuevo;
    (*totLibros)++;

    printf("Libro agregado con ID %d\n", nuevo->id);
    return librosArr;
}

/* Mostrar todos los libros (tabla) */
void mostrarLibros(Libro **librosArr, int totLibros) {
    if (totLibros == 0) { printf("No hay libros registrados.\n"); return; }
    printf("ID | Titulo (Autor) | ISBN | Año | Categoria | Disponible\n");
    printf("--------------------------------------------------------------------------\n");
    for (int i = 0; i < totLibros; i++) {
        Libro *b = librosArr[i];
        printf("%2d | %.30s (%.20s) | %s | %d | %.15s | %s\n",
               b->id, b->titulo, b->autor, b->isbn, b->anio, b->categoria,
               b->disponible ? "SI" : "NO");
    }
}

/* ---------- Gestión de Usuarios ---------- */

/* Buscar índice de usuario por ID */
int indexUsuarioPorId(Usuario **usuariosArr, int tot, int id) {
    for (int i = 0; i < tot; i++) if (usuariosArr[i]->id == id) return i;
    return -1;
}

/* Buscar usuario por matrícula (exacta) */
Usuario* buscarUsuarioPorMatricula(Usuario **usuariosArr, int tot, int matricula) {
    for (int i = 0; i < tot; i++) if (usuariosArr[i]->matricula == matricula) return usuariosArr[i];
    return NULL;
}

/* Agregar usuario (interactivo) */
Usuario** agregarUsuario(Usuario **usuariosArr, int *totUsuarios) {
    Usuario *nuevo = (Usuario*) malloc(sizeof(Usuario));
    if (!nuevo) { printf("Error: memoria insuficiente.\n"); return usuariosArr; }

    printf("Agregar nuevo usuario:\n");
    getchar(); // limpiar \n
    printf("Nombre completo: ");
    fgets(nuevo->nombre, sizeof(nuevo->nombre), stdin);
    nuevo->nombre[strcspn(nuevo->nombre, "\n")] = 0;

    printf("Carrera: ");
    fgets(nuevo->carrera, sizeof(nuevo->carrera), stdin);
    nuevo->carrera[strcspn(nuevo->carrera, "\n")] = 0;

    char matStr[20];
    printf("Matricula (numero): ");
    fgets(matStr, sizeof(matStr), stdin);
    nuevo->matricula = atoi(matStr);

    nuevo->activo = 1;
    nuevo->id = generarId(&ultimoIdUsuario);

    Usuario **temp = (Usuario**) realloc(usuariosArr, sizeof(Usuario*) * ((*totUsuarios) + 1));
    if (!temp) {
        printf("Error: realloc falló.\n");
        free(nuevo);
        return usuariosArr;
    }
    usuariosArr = temp;
    usuariosArr[*totUsuarios] = nuevo;
    (*totUsuarios)++;
    printf("Usuario agregado con ID %d\n", nuevo->id);
    return usuariosArr;
}

/* Mostrar usuarios */
void mostrarUsuarios(Usuario **usuariosArr, int totUsuarios) {
    if (totUsuarios == 0) { printf("No hay usuarios registrados.\n"); return; }
    printf("ID | Nombre | Carrera | Matricula | Activo\n");
    printf("-----------------------------------------------------------\n");
    for (int i = 0; i < totUsuarios; i++) {
        Usuario *u = usuariosArr[i];
        printf("%2d | %.25s | %.18s | %d | %s\n",
               u->id, u->nombre, u->carrera, u->matricula, u->activo ? "SI" : "NO");
    }
}

/* ---------- Gestión de Préstamos ---------- */

/* Contar prestamos activos de un usuario */
int prestamosActivosPorUsuario(Prestamo **prestArr, int totPrest, int idUsuario) {
    int cuenta = 0;
    for (int i = 0; i < totPrest; i++) {
        if (prestArr[i]->id_usuario == idUsuario && prestArr[i]->devuelto == 0) cuenta++;
    }
    return cuenta;
}

/* Buscar índice préstamo por ID */
int indexPrestamoPorId(Prestamo **prestArr, int tot, int id) {
    for (int i = 0; i < tot; i++) if (prestArr[i]->id == id) return i;
    return -1;
}

/* Realizar préstamo (interactivo) */
Prestamo** realizarPrestamo(Prestamo **prestArr, int *totPrestArr, Libro **librosArr, int totLibrosArr, Usuario **usuariosArr, int totUsuariosArr) {
    char idLibroStr[10], idUsuarioStr[10];
    printf("Realizar préstamo - Ingrese ID del libro: ");
    scanf("%s", idLibroStr);
    int idLibro = atoi(idLibroStr);
    int idxLibro = indexLibroPorId(librosArr, totLibrosArr, idLibro);
    if (idxLibro < 0) { printf("Error: libro no encontrado.\n"); return prestArr; }
    if (librosArr[idxLibro]->disponible == 0) { printf("Error: libro no disponible.\n"); return prestArr; }

    printf("Ingrese ID del usuario: ");
    scanf("%s", idUsuarioStr);
    int idUsuario = atoi(idUsuarioStr);
    int idxUsuario = indexUsuarioPorId(usuariosArr, totUsuariosArr, idUsuario);
    if (idxUsuario < 0) { printf("Error: usuario no encontrado.\n"); return prestArr; }
    if (usuariosArr[idxUsuario]->activo == 0) { printf("Error: usuario inactivo.\n"); return prestArr; }

    int activos = prestamosActivosPorUsuario(prestArr, *totPrestArr, idUsuario);
    if (activos >= 3) { printf("Error: el usuario ya tiene 3 préstamos activos.\n"); return prestArr; }

    /* crear préstamo */
    Prestamo *nuevo = (Prestamo*) malloc(sizeof(Prestamo));
    if (!nuevo) { printf("Error: memoria insuficiente.\n"); return prestArr; }
    nuevo->id = generarId(&ultimoIdPrestamo);
    nuevo->id_libro = idLibro;
    nuevo->id_usuario = idUsuario;
    obtenerFechaActual(nuevo->fecha_prestamo, sizeof(nuevo->fecha_prestamo));
    calcularFechaDevolucion(nuevo->fecha_prestamo, nuevo->fecha_devolucion);
    strcpy(nuevo->fecha_devolucion_real, "");
    nuevo->devuelto = 0;

    /* cambiar estado del libro a no disponible */
    librosArr[idxLibro]->disponible = 0;

    Prestamo **temp = (Prestamo**) realloc(prestArr, sizeof(Prestamo*) * ((*totPrestArr) + 1));
    if (!temp) {
        printf("Error: realloc falló.\n");
        free(nuevo);
        return prestArr;
    }
    prestArr = temp;
    prestArr[*totPrestArr] = nuevo;
    (*totPrestArr)++;

    printf("Préstamo registrado con ID %d. Fecha prestamo: %s - Fecha devolución: %s\n",
           nuevo->id, nuevo->fecha_prestamo, nuevo->fecha_devolucion);
    return prestArr;
}

/* Devolver libro por ID de préstamo */
int devolverLibro(Prestamo **prestArr, int totPrestArr, Libro **librosArr, int totLibrosArr, int idPrestamo) {
    int idx = indexPrestamoPorId(prestArr, totPrestArr, idPrestamo);
    if (idx < 0) { printf("Error: préstamo no encontrado.\n"); return 0; }
    Prestamo *p = prestArr[idx];
    if (p->devuelto == 1) { printf("Este préstamo ya fue devuelto.\n"); return 0; }

    /* cambiar estado */
    p->devuelto = 1;
    obtenerFechaActual(p->fecha_devolucion_real, sizeof(p->fecha_devolucion_real));

    /* cambiar estado del libro a disponible */
    int idxLibro = indexLibroPorId(librosArr, totLibrosArr, p->id_libro);
    if (idxLibro >= 0) librosArr[idxLibro]->disponible = 1;

    /* verificar retraso */
    if (compararFechas(p->fecha_devolucion_real, p->fecha_devolucion) > 0) {
        printf("ATENCIÓN: El libro se devolvió con retraso. Fecha devolución real: %s. Fecha límite: %s\n",
               p->fecha_devolucion_real, p->fecha_devolucion);
    } else {
        printf("Devolución registrada correctamente en %s (línea préstamo ID %d)\n",
               p->fecha_devolucion_real, p->id);
    }
    return 1;
}

/* Mostrar préstamos activos (no devueltos), incluir info de libro y usuario y resaltar vencidos */
void mostrarPrestamosActivos(Prestamo **prestArr, int totPrestArr, Libro **librosArr, int totLibrosArr, Usuario **usuariosArr, int totUsuariosArr) {
    int hay = 0;
    char hoy[20];
    obtenerFechaActual(hoy, sizeof(hoy));
    printf("Prestamos activos:\n");
    printf("ID | ID_Libro | TituloLibro | ID_Usr | NombreUsr | FechaPrest | FechaLimite | Vencido?\n");
    printf("-------------------------------------------------------------------------------------------\n");
    for (int i = 0; i < totPrestArr; i++) {
        Prestamo *p = prestArr[i];
        if (p->devuelto == 0) {
            hay = 1;
            int idxL = indexLibroPorId(librosArr, totLibrosArr, p->id_libro);
            int idxU = indexUsuarioPorId(usuariosArr, totUsuariosArr, p->id_usuario);
            const char *titul = (idxL >= 0) ? librosArr[idxL]->titulo : "Desconocido";
            const char *nomu = (idxU >= 0) ? usuariosArr[idxU]->nombre : "Desconocido";
            int vencido = compararFechas(hoy, p->fecha_devolucion) > 0;
            printf("%2d | %7d | %.25s | %6d | %.15s | %s | %s | %s\n",
                   p->id, p->id_libro, titul, p->id_usuario, nomu, p->fecha_prestamo, p->fecha_devolucion,
                   vencido ? "SI" : "NO");
        }
    }
    if (!hay) printf("No hay préstamos activos.\n");
}

/* ---------- Liberar memoria completa ---------- */
void liberarMemoria_completa(Libro **libArr, int totLib, Usuario **usrArr, int totUsr, Prestamo **prestArr, int totPrest) {
    for (int i = 0; i < totLib; i++) {
        free(libArr[i]);
    }
    free(libArr);

    for (int i = 0; i < totUsr; i++) {
        free(usrArr[i]);
    }
    free(usrArr);

    for (int i = 0; i < totPrest; i++) {
        free(prestArr[i]);
    }
    free(prestArr);

    printf("Memoria liberada correctamente.\n");
}

/* ---------- Datos de prueba obligatorios (cargar) ---------- */
void cargarDatosPrueba() {
    /* Cargar libros */
    struct {
        char *titulo; char *autor; char *isbn; int anio; char *cat; int disp;
    } datosLibros[] = {
        {"El lenguaje de programación C", "Brian Kernighan y Dennis Ritchie", "978-0131103627", 1988, "Programacion", 1},
        {"Introducción a los algoritmos", "Thomas H. Cormen", "978-0262033848", 2009, "Algoritmos", 1},
        {"Estructuras de datos y algoritmos", "Alfred V. Aho", "978-0201000238", 1983, "Estructuras de Datos", 1},
        {"Sistemas operativos: conceptos fundamentales", "Abraham Silberschatz", "978-1118063330", 2018, "Sistemas Operativos", 1},
        {"Redes de computadoras", "Andrew S. Tanenbaum", "978-0132126953", 2013, "Redes", 1},
        {"Base de datos: diseño e implementación", "Raghu Ramakrishnan", "978-0072465631", 2002, "Bases de Datos", 1}
    };
    int nLib = sizeof(datosLibros)/sizeof(datosLibros[0]);
    for (int i = 0; i < nLib; i++) {
        Libro *n = (Libro*) malloc(sizeof(Libro));
        n->id = generarId(&ultimoIdLibro);
        strncpy(n->titulo, datosLibros[i].titulo, sizeof(n->titulo)-1);
        strncpy(n->autor, datosLibros[i].autor, sizeof(n->autor)-1);
        strncpy(n->isbn, datosLibros[i].isbn, sizeof(n->isbn)-1);
        n->anio = datosLibros[i].anio;
        strncpy(n->categoria, datosLibros[i].cat, sizeof(n->categoria)-1);
        n->disponible = datosLibros[i].disp;
        Libro **temp = (Libro**) realloc(libros, sizeof(Libro*) * (totalLibros + 1));
        if (!temp) { printf("Error: realloc libros datos prueba.\n"); free(n); continue; }
        libros = temp;
        libros[totalLibros++] = n;
    }

    /* Cargar usuarios (estudiantes) */
    struct {
        char *nombre; char *carrera; int matricula; int activo;
    } datosUsuarios[] = {
        {"Ana García López", "Ingeniería en Software", 2023001, 1},
        {"Carlos Hernández Martínez", "Ciencias de la Computación", 2023002, 1},
        {"María Fernández González", "Tecnologías de la Información", 2023003, 1},
        {"José Rodríguez Pérez", "Ingeniería en Software", 2023004, 1},
        {"Laura Martínez Sánchez", "Ciencias de la Computación", 2023005, 1}
    };
    int nUsr = sizeof(datosUsuarios)/sizeof(datosUsuarios[0]);
    for (int i = 0; i < nUsr; i++) {
        Usuario *u = (Usuario*) malloc(sizeof(Usuario));
        u->id = generarId(&ultimoIdUsuario);
        strncpy(u->nombre, datosUsuarios[i].nombre, sizeof(u->nombre)-1);
        strncpy(u->carrera, datosUsuarios[i].carrera, sizeof(u->carrera)-1);
        u->matricula = datosUsuarios[i].matricula;
        u->activo = datosUsuarios[i].activo;
        Usuario **temp = (Usuario**) realloc(usuarios, sizeof(Usuario*) * (totalUsuarios + 1));
        if (!temp) { printf("Error: realloc usuarios datos prueba.\n"); free(u); continue; }
        usuarios = temp;
        usuarios[totalUsuarios++] = u;
    }

    printf("Datos de prueba cargados: %d libros, %d usuarios.\n", totalLibros, totalUsuarios);
}

/* ---------- Menú y funciones de ayuda ---------- */

void mostrarMenu() {
    printf("\n--- Sistema Biblioteca UACM - Menú ---\n");
    printf("1. Agregar libro\n");
    printf("2. Mostrar libros\n");
    printf("3. Buscar libro por título\n");
    printf("4. Agregar usuario\n");
    printf("5. Mostrar usuarios\n");
    printf("6. Realizar préstamo\n");
    printf("7. Devolver libro (por ID de préstamo)\n");
    printf("8. Mostrar préstamos activos\n");
    printf("9. Cargar datos de prueba (obligatorios)\n");
    printf("10. Liberar memoria y salir\n");
    printf("--------------------------------------\n");
    printf("Elija una opción: ");
}

void buscarLibroTituloInteractivo() {
    char titulo[101];
    getchar();
    printf("Ingrese texto a buscar en títulos: ");
    fgets(titulo, sizeof(titulo), stdin);
    titulo[strcspn(titulo, "\n")] = 0;
    Libro *r = buscarLibroPorTitulo(libros, totalLibros, titulo);
    if (!r) { printf("No se encontró libro con '%s' en el título.\n", titulo); }
    else {
        printf("Encontrado: ID %d - %s (Autor: %s) ISBN: %s Año: %d Disponibilidad: %s\n",
               r->id, r->titulo, r->autor, r->isbn, r->anio, r->disponible ? "SI" : "NO");
    }
}

/* ---------- Main (interactivo) ---------- */
int main() {
    int opcion = 0;
    int running = 1;
    printf("Bienvenido al sistema de gestión de biblioteca (UACM) - Proyecto final\n");

    while (running) {
        mostrarMenu();
        if (scanf("%d", &opcion) != 1) {
            printf("Entrada inválida. Saliendo.\n");
            break;
        }
        switch (opcion) {
            case 1:
                libros = agregarLibro(libros, &totalLibros);
                break;
            case 2:
                mostrarLibros(libros, totalLibros);
                break;
            case 3:
                buscarLibroTituloInteractivo();
                break;
            case 4:
                usuarios = agregarUsuario(usuarios, &totalUsuarios);
                break;
            case 5:
                mostrarUsuarios(usuarios, totalUsuarios);
                break;
            case 6:
                prestamos = realizarPrestamo(prestamos, &totalPrestamos, libros, totalLibros, usuarios, totalUsuarios);
                break;
            case 7: {
                char idp[10];
                printf("Ingrese ID del préstamo a devolver: ");
                scanf("%s", idp);
                int idPrest = atoi(idp);
                devolverLibro(prestamos, totalPrestamos, libros, totalLibros, idPrest);
                break;
            }
            case 8:
                mostrarPrestamosActivos(prestamos, totalPrestamos, libros, totalLibros, usuarios, totalUsuarios);
                break;
            case 9:
                cargarDatosPrueba();
                break;
            case 10:
                liberarMemoria_completa(libros, totalLibros, usuarios, totalUsuarios, prestamos, totalPrestamos);
                running = 0;
                break;
            default:
                printf("Opción no válida.\n");
        }
    }

    return 0;
}
