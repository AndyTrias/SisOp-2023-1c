#include <solicitudes.h>

char* nombre_archivo;

void atender_solicitudes(int cod_op, t_parametros_variables *parametros_instruccion)
{

    nombre_archivo = parametros_instruccion->parametros[0];

    switch (cod_op)
    {
    case F_CREATE:
        log_info(LOGGER_FILE_SYSTEM, "Crear Archivo: %s", nombre_archivo);
        crear_fcb(nombre_archivo);

        // Devolver OK a kernel
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

        void *archivo_de_bloques = mmap(NULL, (TAMANIO_BLOQUES * CANTIDAD_BLOQUES), PROT_READ | PROT_WRITE, MAP_SHARED, fileno(ARCHIVO_BLOQUES), 0);

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

    case F_READ:
        log_info(LOGGER_FILE_SYSTEM, "Leer Archivo: %s - Puntero: %s - Memoria: %s - Tamaño: %s", nombre_archivo, parametros_instruccion->parametros[3], parametros_instruccion->parametros[1], parametros_instruccion->parametros[2]);
        break;

    case F_WRITE:
        log_info(LOGGER_FILE_SYSTEM, "Escribir Archivo: %s - Puntero: %s - Memoria: %s - Tamaño: %s", nombre_archivo, parametros_instruccion->parametros[3], parametros_instruccion->parametros[1], parametros_instruccion->parametros[2]);
        break;

    default:
        log_error(LOGGER_FILE_SYSTEM, "Operacion desconocida");
        return;
    }
}

void agrandar_archivo(void* archivo_de_bloques, int cantidad_bloques_a_agregar, int actuales, bool nuevo_archivo)
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
        // Hacer el log de acceso a bloque
        // Hacer el retardo
        void *bloque_puntero_indirecto = archivo_de_bloques + TAMANIO_BLOQUES * obtener_puntero_indirecto(nombre_archivo);
        asignar_bloques_al_puntero_indirecto(bloque_puntero_indirecto, cantidad_bloques_a_agregar, actuales);
    }
}

void achicar_archivo(void* archivo_de_bloques, int cantidad_bloques_a_liberar, int actuales, bool borrar_todo)
{
    if (borrar_todo)
    {
        liberar_puntero_directo(nombre_archivo);
        cantidad_bloques_a_liberar--;
    }

    if (cantidad_bloques_a_liberar > 0)
    {
        // Hacer el log de acceso a bloque
        // Hacer el retardo
        void *bloque_puntero_indirecto = archivo_de_bloques + TAMANIO_BLOQUES * obtener_puntero_indirecto(nombre_archivo);
        liberar_bloques_del_puntero_indirecto(bloque_puntero_indirecto, cantidad_bloques_a_liberar, actuales);
    }

    if (borrar_todo)
    {
        liberar_puntero_indirecto(nombre_archivo);
    }
}
