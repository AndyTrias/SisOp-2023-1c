#include <solicitudes.h>

void atender_solicitudes(int cod_op, t_parametros_variables *parametros_instruccion)
{

    char *nombre_archivo = parametros_instruccion->parametros[0];

    char* path_archivo = string_from_format("%s/%s", path_fcb, nombre_archivo);

    switch (cod_op)
    {
    case F_CREATE:
        t_config* config_nuevo_archivo = crear_fcb();
        dictionary_put(DICCIONARIO_FCB, nombre_archivo, iniciar_config(nombre_archivo));

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


t_config *crear_fcb()
{
    t_config *nuevo = iniciar_config(path_archivo);

    config_set_value(nuevo, "NOMBRE_ARCHIVO", nombre_archivo);
    config_set_value(nuevo, "TAMANIO_ARCHIVO", "0");

    
    config_set_value(nuevo, "PUNTERO_DIRECTO", "0");
    config_set_value(nuevo, "PUNTERO_INDIRECTO", "0");
        
    config_save(nuevo)
    return nuevo;
}