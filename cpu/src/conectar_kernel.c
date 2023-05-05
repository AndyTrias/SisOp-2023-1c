#include "conectar_kernel.h"

void conectar_kernel(int socket_servidor)
{
    log_info(LOGGER_CPU, "Esperando kernel....");
    while (1)
    {
        int socket_kernel = esperar_cliente(socket_servidor);
        log_info(LOGGER_CPU, "Se conecto el kernel");
        enviado_de_kernel(&socket_kernel);
    }
}

void enviado_de_kernel(int *socket_kernel)
{
    while (1)
    {
        int cod_op = recibir_operacion(*socket_kernel);
        switch (cod_op)
        {
        case CONTEXTO:
            log_info(LOGGER_CPU, "Se recibio un contexto del kernel");
            t_ctx* ctx = recibir_paquete_kernel(*socket_kernel);
            break;

        case -1:
            log_info(LOGGER_CPU, "Se desconecto el kernel");
            return;

        default:
            log_error(LOGGER_CPU, "Operacion desconocida");
            return;
        }
    }
}

t_ctx* recibir_paquete_kernel(int socket_kernel)
{
    int size;
    void *buffer = recibir_buffer(&size, socket_kernel);

    t_ctx *ctx = deserializar_contexto(buffer);

    return ctx;
}