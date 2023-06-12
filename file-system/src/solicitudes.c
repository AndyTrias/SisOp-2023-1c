#include <solicitudes.h>

void atender_solicitudes(int cod_op, t_parametros_variables *parametros_instruccion)
{
    switch (cod_op)
    {
    case F_CREATE:
        break;

    case F_OPEN:
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