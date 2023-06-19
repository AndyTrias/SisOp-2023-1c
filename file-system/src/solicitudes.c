#include <solicitudes.h>

void atender_solicitudes(int cod_op, t_parametros_variables *parametros_instruccion)
{

    char *nombre_archivo = parametros_instruccion->parametros[0];

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
        log_info(LOGGER_FILE_SYSTEM, "Truncar Archivo: %s - Tamaño: %d", nombre_archivo, tamanio_nuevo);

        int cantidad_bloques_actual = (tamanio_actual + TAMANIO_BLOQUES - 1) / TAMANIO_BLOQUES;
        int cantidad_bloques_necesarios = (tamanio_nuevo + TAMANIO_BLOQUES - 1) / TAMANIO_BLOQUES;

        void *mapeo = mmap(NULL, (TAMANIO_BLOQUES * CANTIDAD_BLOQUES), PROT_READ | PROT_WRITE, MAP_SHARED, fileno(ARCHIVO_BLOQUES), 0);
        void *bloque_puntero_indirecto = mapeo + TAMANIO_BLOQUES * obtener_puntero_indirecto(nombre_archivo);

        // Se agranda el archivo
        if (tamanio_nuevo > tamanio_actual)
        {
            int cantidad_bloques_a_agregar = cantidad_bloques_necesarios - cantidad_bloques_actual;

            // El puntero directo siempre se mantiene
            // Se asigna un bloque menos
            if (tamanio_actual < TAMANIO_BLOQUES)
            {
                cantidad_bloques_a_agregar--;
            }

            // Asignar todos los bloques libres en el bitmap de bloques
            // Asignar esos bloques libres en el archivo de bloques
            asignar_bloques_al_puntero_indirecto(bloque_puntero_indirecto, cantidad_bloques_necesarios, tamanio_actual);
        }

        // Se achica el archivo
        else
        {
            int cantidad_bloques_a_liberar = cantidad_bloques_actual - cantidad_bloques_necesarios;

            // Si el nuevo tamanio es menor a 64
            // el puntero directo se mantiene
            if (tamanio_nuevo < TAMANIO_BLOQUES)
            {
                cantidad_bloques_a_liberar--;
            }

            liberar_bloques_del_puntero_indirecto(bloque_puntero_indirecto, cantidad_bloques_a_liberar, tamanio_actual);
        }

        msync(mapeo, CANTIDAD_BLOQUES * TAMANIO_BLOQUES, MS_SYNC);
        munmap(mapeo, CANTIDAD_BLOQUES * TAMANIO_BLOQUES);
        actualizar_fcb(nombre_archivo, "TAMANIO_ARCHIVO", parametros_instruccion->parametros[1]);
        break;

    case F_READ:
        log_info(LOGGER_FILE_SYSTEM, "Leer Archivo: %s - Puntero: %s - Memoria: %s - Tamaño: %s", nombre_archivo, parametros_instruccion->parametros[1], parametros_instruccion->parametros[2], parametros_instruccion->parametros[3]);
        break;

    case F_WRITE:
        log_info(LOGGER_FILE_SYSTEM, "Escribir Archivo: %s - Puntero: %s - Memoria: %s - Tamaño: %s", nombre_archivo, parametros_instruccion->parametros[1], parametros_instruccion->parametros[2], parametros_instruccion->parametros[3]);
        break;

    default:
        log_error(LOGGER_FILE_SYSTEM, "Operacion desconocida");
        return;
    }
}


