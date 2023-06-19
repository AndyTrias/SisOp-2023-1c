#include <solicitudes.h>

void atender_solicitudes(int cod_op, t_parametros_variables *parametros_instruccion)
{

    char *nombre_archivo = parametros_instruccion->parametros[0];

    switch (cod_op)
    {
    case F_CREATE:
        crear_fcb(nombre_archivo);

        // Devolver OK a kernel
        break;

    case F_OPEN:
        if (dictionary_get(DICCIONARIO_FCB, nombre_archivo) == NULL)
            log_info(LOGGER_FILE_SYSTEM, "El archivo no existe");
        // Devolver que no existe a kernel
        else
            log_info(LOGGER_FILE_SYSTEM, "El archivo existe");
        // Devolver que existe a kernel
        break;

    case F_TRUNCATE:

        t_config *fcb = dictionary_get(DICCIONARIO_FCB, nombre_archivo);
        int tamanio_actual = config_get_int_value(fcb, "TAMANIO_ARCHIVO");
        int block_size = 64;
        int block_count = 65536; 
        int cantidad_punteros_actual = (tamanio_actual -1) / block_size;
        
        int nuevo_tamanio = atoi(parametros_instruccion->parametros[1]);

        if (nuevo_tamanio > tamanio_actual)
        {
            int tamanio_necesario = nuevo_tamanio - tamanio_actual;

            int cantidad_bloques_necesarios = (tamanio_necesario + block_size - 1) / block_size;

            // Ya tiene asignado el puntero directo
            // Se asigna un bloque menos
            if (tamanio_actual < block_size)
            {
                cantidad_bloques_necesarios--;
            }

            // asignar todos los bloques libres en el bitmap de bloques
            // asignar esos bloques libres en el archivo de bloques
            void *mapeo = mmap(NULL, block_size * block_count, PROT_READ | PROT_WRITE, MAP_SHARED, fileno(ARCHIVO_BLOQUES), 0);
            void* bloque_puntero_indirecto = mapeo + block_size * config_get_int_value(fcb, "PUNTERO_INDIRECTO");

            // calculo la cantidad de punteros que tiene ese bloques para ir a esa posicion


            int i;
            u_int32_t bloque_libre;
            for (i = 0; i < cantidad_bloques_necesarios; i++)
            {
                bloque_libre = buscar_bloque_libre();
                if (bloque_libre == -1)
                {
                    log_error(LOGGER_FILE_SYSTEM, "No hay bloques libres");
                    return;
                }
                else{
                    memcpy((cantidad_punteros_actual + i) * sizeof(u_int32_t) + bloque_puntero_indirecto, &bloque_libre, sizeof(uint32_t));
                }
            }
            msync(mapeo, block_size * block_count, MS_SYNC);
            munmap(mapeo, block_size * block_count);
        }

        actualizar_fcb(nombre_archivo, "TAMANIO_ARCHIVO", parametros_instruccion->parametros[1]);
        break;

    case F_READ:
        log_info(LOGGER_FILE_SYSTEM, "Se recibio un mensaje del kernel");
        break;

    case F_WRITE:
        log_info(LOGGER_FILE_SYSTEM, "Se recibio un mensaje del kernel");
        break;

    default:
        log_error(LOGGER_FILE_SYSTEM, "Operacion desconocida");
        return;
    }
}

u_int32_t buscar_bloque_libre()
{
    int i;
    for (i = 0; i < bitarray_get_max_bit(BITMAP_BLOQUES); i++)
    {
        if (bitarray_test_bit(BITMAP_BLOQUES, i) == 0)
        {
            bitarray_set_bit(BITMAP_BLOQUES, i);
            return i;
        }
    }
    return -1;
}
