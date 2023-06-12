#include "conectar_modulos.h"

void conectar_modulos(int socket_servidor) {
    log_info(LOGGER_MEMORIA, "Esperando conexiones de modulos...");
    
    pthread_t hilo_kernel, hilo_cpu, hilo_fs;
    int socket_kernel, socket_cpu, socket_fs, socket_cliente;

    for (int i = 0; i < 3; i++) {
        socket_cliente = esperar_cliente(socket_servidor);
        switch(recibir_operacion(socket_cliente)) {
            case 0:
                log_info(LOGGER_MEMORIA, "Se conecto el file system");
                socket_fs = socket_cliente;
                pthread_create(&hilo_fs, NULL, (void *) nuevo_modulo, &socket_fs);
                pthread_detach(hilo_fs);
                break;
            case 1:
                log_info(LOGGER_MEMORIA, "Se conecto el kernel");
                socket_kernel = socket_cliente;
                pthread_create(&hilo_kernel, NULL, (void *) recibir_kernel, &socket_kernel);
                pthread_join(hilo_kernel, NULL);
                break;
            case 2:
                log_info(LOGGER_MEMORIA, "Se conecto el cpu");
                socket_cpu = socket_cliente;
                pthread_create(&hilo_cpu, NULL, (void *) nuevo_modulo, &socket_cpu);
                pthread_detach(hilo_cpu);
                break;
        }
    }

}

void recibir_kernel(int *socket_modulo)
{
    while (1)
    {
        int cod_op = recibir_operacion(*socket_modulo);
        switch (cod_op)
        {
        case CREAR_TABLA_SEGMENTOS:
            //recibe
            t_ctx* ctx = recibir_paquete_kernel(*socket_modulo);

            // crea
            t_list *tabla_segmentos = crear_tabla_segmentos();
            log_info(LOGGER_MEMORIA, "Creación de Proceso PID: <%d>", ctx->PID);

            // envia
            t_paquete *paquete = crear_paquete(CREAR_TABLA_SEGMENTOS);
            serializar_tabla_segmentos(tabla_segmentos, paquete);
            enviar_paquete(paquete, *socket_modulo);
            free(paquete);
            free(ctx);

            break;

        case TERMINAR:
            // recibe
            ctx = recibir_paquete_kernel(*socket_modulo);

            // elimina
            finalizar_proceso(ctx->tabla_segmentos);
            log_info(LOGGER_MEMORIA, "Eliminación de Proceso PID: <%d>", ctx->PID);

            free(ctx);
            break;

        case CREATE_SEGMENT:
            // recibe
            ctx = recibir_paquete_kernel(*socket_modulo);

            // crea
            void* base = crear_segmento(atoi(ctx->motivos_desalojo->parametros[0]), atoi(ctx->motivos_desalojo->parametros[1]));
            log_info(LOGGER_MEMORIA, "PID: <%d> - Crear Segmento: <%d> - Base: <%p> - TAMAÑO: <%d>", ctx->PID, atoi(ctx->motivos_desalojo->parametros[0]), base, atoi(ctx->motivos_desalojo->parametros[1]));
            
            // envia
            paquete = crear_paquete(CREATE_SEGMENT);
            agregar_a_paquete_dato_serializado(paquete, &base, sizeof(void*));
            enviar_paquete(paquete, *socket_modulo);
            free(paquete);
            free(ctx);

            break;
        case DELETE_SEGMENT:
            // recibe
            ctx = recibir_paquete_kernel(*socket_modulo);

            // elimina
            eliminar_segmento(ctx->tabla_segmentos, atoi(ctx->motivos_desalojo->parametros[0]), ctx->PID);
            // log obligatorio en eliminar segmento

            // envia
            paquete = crear_paquete(DELETE_SEGMENT);
            serializar_tabla_segmentos(tabla_segmentos, paquete);
            enviar_paquete(paquete, *socket_modulo);
            free(paquete);
            free(ctx);
            break;

        case -1:
            log_info(LOGGER_MEMORIA, "Se desconecto un modulo");
            return;

        default:
            log_error(LOGGER_MEMORIA, "Operacion desconocida");
            break;
        }
    }
}

t_ctx* recibir_paquete_kernel(int socket_kernel)
{
    int size;
    void *buffer = recibir_buffer(&size, socket_kernel);

    int *desplazamiento = malloc(sizeof(int));
    *desplazamiento = 0; 
    
    t_ctx* ctx = deserializar_contexto(buffer, desplazamiento);
    
    free(desplazamiento);
    return ctx;
}

void nuevo_modulo(int* socket_modulo) {
    while(1) {
        int cod_op = recibir_operacion(*socket_modulo);
        switch(cod_op) {
            
            case MENSAJE:
                char* mensaje_recibido = recibir_mensaje(*socket_modulo);
                log_info(LOGGER_MEMORIA, "Me llego el mensaje: %s", mensaje_recibido);
                free(mensaje_recibido);
                break;
            
            case -1:
                log_info(LOGGER_MEMORIA, "Se desconecto un modulo");
                return;
            
            default:
                log_error(LOGGER_MEMORIA, "Operacion desconocida");
                return;
        }
    }
}