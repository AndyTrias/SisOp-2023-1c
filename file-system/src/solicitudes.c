#include <solicitudes.h>

char *nombre_archivo;
void* archivo_de_bloques;

void atender_solicitudes(int cod_op, t_parametros_variables *parametros_instruccion)
{

    nombre_archivo = parametros_instruccion->parametros[0];

    switch (cod_op)
    {
    case F_CREATES:
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

    case F_READ:
        int posicion_archivo = atoi(parametros_instruccion->parametros[3]);
        int tamanio_a_leer = atoi(parametros_instruccion->parametros[2]);
        char* memoria = parametros_instruccion->parametros[1];
        log_info(LOGGER_FILE_SYSTEM, "Leer Archivo: %s - Puntero: %d - Memoria: %s - Tamaño: %d", nombre_archivo, posicion_archivo, memoria, tamanio_a_leer);

        char *buffer = string_new();
        int offset = posicion_archivo % TAMANIO_BLOQUES;

        archivo_de_bloques = mmap(NULL, (TAMANIO_BLOQUES * CANTIDAD_BLOQUES), PROT_READ | PROT_WRITE, MAP_SHARED, fileno(ARCHIVO_BLOQUES), 0);
        int tamanio_a_leer_del_bloque;

        // leemos el puntero directo
        if (posicion_archivo < 64)
        {
            tamanio_a_leer_del_bloque = MIN(tamanio_a_leer, TAMANIO_BLOQUES - offset);
            string_append(&buffer, leer_bloque(archivo_de_bloques, obtener_puntero_directo(nombre_archivo), offset, tamanio_a_leer));
            
            offset = 0;
            posicion_archivo += tamanio_a_leer_del_bloque;
            tamanio_a_leer -= tamanio_a_leer_del_bloque;
        }

        if (tamanio_a_leer > 0)
        {
            void *puntero_indirecto = cargar_bloque_indirecto(archivo_de_bloques, nombre_archivo);
        
            while (tamanio_a_leer > 0)
            {
                int numero_de_bloque = posicion_archivo / TAMANIO_BLOQUES;
                tamanio_a_leer_del_bloque = MIN(tamanio_a_leer, TAMANIO_BLOQUES - offset);
                offset = 0;

                int bloque = leer_puntero_indirecto(puntero_indirecto, numero_de_bloque);
 
                string_append(&buffer, leer_bloque(archivo_de_bloques, bloque, offset, tamanio_a_leer_del_bloque));

                tamanio_a_leer -= tamanio_a_leer_del_bloque;
            }
        }

        // Escribir el buffer en la memoria

        free(memoria);
        msync(archivo_de_bloques, CANTIDAD_BLOQUES * TAMANIO_BLOQUES, MS_SYNC);
        munmap(archivo_de_bloques, CANTIDAD_BLOQUES * TAMANIO_BLOQUES);
        break;

    case F_WRITE:
        log_info(LOGGER_FILE_SYSTEM, "Escribir Archivo: %s - Puntero: %s - Memoria: %s - Tamaño: %s", nombre_archivo, parametros_instruccion->parametros[3], parametros_instruccion->parametros[1], parametros_instruccion->parametros[2]);
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

        void *bloque_puntero_indirecto = cargar_bloque_indirecto(archivo_de_bloques, nombre_archivo);
        asignar_bloques_al_puntero_indirecto(bloque_puntero_indirecto, cantidad_bloques_a_agregar, actuales);
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
        void *bloque_puntero_indirecto = cargar_bloque_indirecto(archivo_de_bloques, nombre_archivo);
        liberar_bloques_del_puntero_indirecto(bloque_puntero_indirecto, cantidad_bloques_a_liberar, actuales);
    }

    if (borrar_todo)
    {
        liberar_puntero_indirecto(nombre_archivo);
    }
}
