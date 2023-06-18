#include <solicitudes.h>

void atender_solicitudes(int cod_op, t_parametros_variables *parametros_instruccion)
{

    char *nombre_archivo = parametros_instruccion->parametros[0];

    switch (cod_op)
    {
    case F_CREATE:
        crear_fcb(nombre_archivo);
        break;

    case F_OPEN:
        if (dictionary_get(DICCIONARIO_FCB, nombre_archivo) == NULL)
            log_info(LOGGER_FILE_SYSTEM, "El archivo no existe");

        else
            log_info(LOGGER_FILE_SYSTEM, "El archivo existe");

        break;

    case F_TRUNCATE:
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



int buscar_bloque_libre()
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

