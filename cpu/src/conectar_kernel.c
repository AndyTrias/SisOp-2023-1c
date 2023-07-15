#include "conectar_kernel.h"

void conectar_kernel(int socket_servidor)
{
    log_info(LOGGER_CPU, "Esperando kernel....");
    SOCKET_KERNEL = esperar_cliente(socket_servidor); //En la librería faltaba utils/conexiones.h, ya no hace falta agreagarlo porque se encuentra en instruccionescpu.h
    log_info(LOGGER_CPU, "Se conecto el kernel");
    enviado_de_kernel(&SOCKET_KERNEL);
    
}

void enviado_de_kernel(int *socket_kernel)
{
    while (1)
    {
        int cod_op = recibir_operacion(*socket_kernel);
        switch (cod_op)
        {
        case CONTEXTO:
            t_ctx* ctx = recibir_paquete_kernel(*socket_kernel);
            ciclo_de_instruccion(ctx); // Esto lo uso para ir probando ahora.
            list_clean(ctx->instrucciones);
            list_destroy(ctx->instrucciones);
            list_clean(ctx->tabla_segmentos);
            list_destroy(ctx->tabla_segmentos);
            free(ctx->motivos_desalojo);
            free(ctx);
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

    int *desplazamiento = malloc(sizeof(int));
    *desplazamiento = 0;

    t_ctx *ctx = deserializar_contexto(buffer, desplazamiento);
    
    return ctx;
}