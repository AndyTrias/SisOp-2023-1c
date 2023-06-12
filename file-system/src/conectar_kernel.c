#include "conectar_kernel.h"

void conectar_kernel(int socket_servidor)
{
    log_info(LOGGER_FILE_SYSTEM, "Esperando kernel....");
    int socket_kernel = esperar_cliente(socket_servidor);
    log_info(LOGGER_FILE_SYSTEM, "Se conecto el kernel");
    enviado_de_kernel(&socket_kernel);
}

void enviado_de_kernel(int *socket_kernel)
{
    while (1)
    {
        int cod_op = recibir_operacion(*socket_kernel);

        if (cod_op == -1)
        {
            log_info(LOGGER_FILE_SYSTEM, "Se desconecto el kernel");
            return;
        }

        t_parametros_variables *parametros_instruccion = recibir_paquete_kernel(*socket_kernel);
        atender_solicitudes(cod_op, parametros_instruccion);
    }
}

t_parametros_variables *recibir_paquete_kernel(int socket_kernel)
{
    int size;
    void *buffer = recibir_buffer(&size, socket_kernel);

    int *desplazamiento = malloc(sizeof(int));
    *desplazamiento = 0;

    t_parametros_variables *parametros = deserealizar_motivos_desalojo(buffer, desplazamiento);

    return parametros;
}
