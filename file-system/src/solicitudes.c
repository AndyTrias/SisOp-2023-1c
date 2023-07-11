#include <solicitudes.h>

char *nombre_archivo;
void *archivo_de_bloques;
int posicion_archivo;
char *memoria;

void atender_solicitudes(int cod_op, t_parametros_variables *parametros_instruccion)
{

    nombre_archivo = parametros_instruccion->parametros[0];

    switch (cod_op)
    {
    case F_CREATES:
        log_info(LOGGER_FILE_SYSTEM, "Crear Archivo: %s", nombre_archivo);
        crear_fcb(nombre_archivo);

        // Devolver un existe, lo hicimos asi
        break;

    case F_OPEN:
        log_info(LOGGER_FILE_SYSTEM, "Abrir Archivo: %s", nombre_archivo);
        if (dictionary_get(DICCIONARIO_FCB, nombre_archivo) == NULL)
            log_info(LOGGER_FILE_SYSTEM, "El archivo no existe");
        // Devolver que no existe a kernel
        else
            log_info(LOGGER_FILE_SYSTEM, "El archivo existe");
        // Devolver que existe a kernel
        break;

    case F_TRUNCATE:
        int tamanio_nuevo = atoi(parametros_instruccion->parametros[1]);
        int tamanio_actual = obtener_tamanio(nombre_archivo);
        actualizar_fcb(nombre_archivo, "TAMANIO_ARCHIVO", parametros_instruccion->parametros[1]);
        log_info(LOGGER_FILE_SYSTEM, "Truncar Archivo: %s - Tamaño: %d", nombre_archivo, tamanio_nuevo);

        // No se tiene en cuenta el puntero indirecto
        int cantidad_bloques_actual = (tamanio_actual + TAMANIO_BLOQUES - 1) / TAMANIO_BLOQUES;
        int cantidad_bloques_necesarios = (tamanio_nuevo + TAMANIO_BLOQUES - 1) / TAMANIO_BLOQUES;

        if (cantidad_bloques_actual == cantidad_bloques_necesarios)
        {
            // No se hace nada
            log_info(LOGGER_FILE_SYSTEM, "No se modifican bloques");
            break;
        }

        archivo_de_bloques = mmap(NULL, (TAMANIO_BLOQUES * CANTIDAD_BLOQUES), PROT_READ | PROT_WRITE, MAP_SHARED, fileno(ARCHIVO_BLOQUES), 0);

        // Se agranda el archivo
        if (cantidad_bloques_necesarios > cantidad_bloques_actual)
        {
            int cantidad_bloques_a_agregar = cantidad_bloques_necesarios - cantidad_bloques_actual;

            agrandar_archivo(archivo_de_bloques, cantidad_bloques_a_agregar, cantidad_bloques_actual, tamanio_actual == 0);
        }

        // Se achica el archivo
        else
        {
            int cantidad_bloques_a_liberar = cantidad_bloques_actual - cantidad_bloques_necesarios;

            achicar_archivo(archivo_de_bloques, cantidad_bloques_a_liberar, cantidad_bloques_actual, tamanio_nuevo == 0);
        }

        msync(archivo_de_bloques, CANTIDAD_BLOQUES * TAMANIO_BLOQUES, MS_SYNC);
        munmap(archivo_de_bloques, CANTIDAD_BLOQUES * TAMANIO_BLOQUES);
        break;


    case F_WRITE:
    case F_READ:
        char *memoria = parametros_instruccion->parametros[1];
        int posicion = atoi(parametros_instruccion->parametros[3]);
        int tamaño = atoi(parametros_instruccion->parametros[2]);

        if (cod_op == F_WRITE)
            f_write(memoria, posicion, tamaño);

        else
            f_read(memoria, posicion, tamaño);

        break;

    default:
        log_error(LOGGER_FILE_SYSTEM, "Operacion desconocida");
        return;
    }
}

void agrandar_archivo(void *archivo_de_bloques, int cantidad_bloques_a_agregar, int actuales, bool nuevo_archivo)
{

    // Si es la primera vez que se llama a truncate
    // Se asigna el puntero directo e indirecto
    if (nuevo_archivo)
    {
        asignar_puntero_directo(nombre_archivo);
        asignar_puntero_indirecto(nombre_archivo);
        cantidad_bloques_a_agregar--;
    }

    if (cantidad_bloques_a_agregar > 0)
    {

        cargar_puntero_indirecto(nombre_archivo);
        asignar_bloques_al_puntero_indirecto(nombre_archivo, cantidad_bloques_a_agregar, actuales);
    }
}

void achicar_archivo(void *archivo_de_bloques, int cantidad_bloques_a_liberar, int actuales, bool borrar_todo)
{
    if (borrar_todo)
    {
        liberar_puntero_directo(nombre_archivo);
        cantidad_bloques_a_liberar--;
    }

    if (cantidad_bloques_a_liberar > 0)
    {
        cargar_puntero_indirecto(nombre_archivo);
        liberar_bloques_del_puntero_indirecto(nombre_archivo, cantidad_bloques_a_liberar, actuales);
    }

    if (borrar_todo)
    {
        liberar_puntero_indirecto(nombre_archivo);
    }
}

void f_write(char *direccion, int posicion_archivo, int tamanio_a_escribir)
{
    char *valor_leido = "Loremipsumdolorsitamet,consecteturadipiscingelit.Sedsuscipitturpisvelullamcorper13fghgff";
    // char* valor_leido = leer_direccion_de_memoria(direccion);
    
    log_info(LOGGER_FILE_SYSTEM, "Escribir Archivo: %s - Puntero: %d - Memoria: %s - Tamaño: %d", nombre_archivo, posicion_archivo, direccion, tamanio_a_escribir);

    if (tamanio_a_escribir + posicion_archivo > 64)
    {
        // Se carga el puntero indirecto en la memoria
        cargar_puntero_indirecto(nombre_archivo);
    }

    while (tamanio_a_escribir > 0)
    {
        int tamanio_a_escribir_del_bloque = MIN(tamanio_a_escribir, TAMANIO_BLOQUES - posicion_archivo % TAMANIO_BLOQUES);
        int bloque_archivo = posicion_archivo / TAMANIO_BLOQUES;
        int offset = posicion_archivo % TAMANIO_BLOQUES;

        escribir_bloque(nombre_archivo, bloque_archivo, offset, tamanio_a_escribir_del_bloque, valor_leido);
        valor_leido = string_substring_from(valor_leido, tamanio_a_escribir_del_bloque);

        posicion_archivo += tamanio_a_escribir_del_bloque;
        tamanio_a_escribir -= tamanio_a_escribir_del_bloque;
    }


    munmap(archivo_de_bloques, CANTIDAD_BLOQUES * TAMANIO_BLOQUES);
}

void f_read(char *direccion, int posicion_archivo, int tamanio_a_leer)
{
    char *buffer = string_new();
    
    log_info(LOGGER_FILE_SYSTEM, "Leer Archivo: %s - Puntero: %d - Memoria: %s - Tamaño: %d", nombre_archivo, posicion_archivo, direccion, tamanio_a_leer);

    if (tamanio_a_leer + posicion_archivo > 64)
    {
        // Se carga el puntero indirecto en la memoria
        cargar_puntero_indirecto(nombre_archivo);
    }

    while (tamanio_a_leer > 0)
    {
        int tamanio_a_leer_del_bloque = MIN(tamanio_a_leer, TAMANIO_BLOQUES - posicion_archivo % TAMANIO_BLOQUES);
        int bloque_archivo = posicion_archivo / TAMANIO_BLOQUES;
        int offset = posicion_archivo % TAMANIO_BLOQUES;

        void *leido = leer_bloque(nombre_archivo, bloque_archivo, offset, tamanio_a_leer_del_bloque);
        string_append(&buffer, (char*) leido);

        posicion_archivo += tamanio_a_leer_del_bloque;
        tamanio_a_leer -= tamanio_a_leer_del_bloque;
    }

    // escribir_valor_en_memoria(buffer, direccion);

    log_info(LOGGER_FILE_SYSTEM, "Valor Leido: %s", buffer);
    munmap(archivo_de_bloques, CANTIDAD_BLOQUES * TAMANIO_BLOQUES);
}