#include "conectar_modulos.h"

void conectar_modulos(int socket_servidor) {
    log_info(LOGGER_MEMORIA, "Esperando conexiones de modulos...");
    
    pthread_t hilo_kernel, hilo_cpu, hilo_fs;
    int socket_cpu, socket_fs, socket_cliente;

    for (int i = 0; i < 3; i++) {
        socket_cliente = esperar_cliente(socket_servidor);
        switch(recibir_operacion(socket_cliente)) {
            case 0:
                log_info(LOGGER_MEMORIA, "Se conecto el file system");
                socket_fs = socket_cliente;
                pthread_create(&hilo_fs, NULL, (void *) recibir_fs, &socket_fs);
                pthread_detach(hilo_fs);
                break;
            case 1:
                log_info(LOGGER_MEMORIA, "Se conecto el kernel");
                SOCKET_KERNEL = socket_cliente;
                pthread_create(&hilo_kernel, NULL, (void *) recibir_kernel, &SOCKET_KERNEL);
                pthread_join(hilo_kernel, NULL);
                break;
            case 2:
                log_info(LOGGER_MEMORIA, "Se conecto el cpu");
                socket_cpu = socket_cliente;
                pthread_create(&hilo_cpu, NULL, (void *) recibir_cpu, &socket_cpu);
                pthread_detach(hilo_cpu);
                break;
        }
    }

}

void imprimir_hueco(t_hueco *hueco)
{
    printf("Base: %p, Tamanio: %d, Libre: %d\n", hueco->base, hueco->tamanio, hueco->libre);
}

void imprimir_segmento(t_segmento *segmento)
{
    printf("Base: %p, Tamanio: %d, Numero de segmento: %d\n", segmento->base, segmento->tamanio, segmento->id_segmento);
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
            int PID = recibir_int(*socket_modulo);

            // crea
            t_list *tabla_segmentos = crear_tabla_segmentos();
            log_info(LOGGER_MEMORIA, "Creación de Proceso PID: <%d>", PID);

            // envia
            t_paquete *paquete = crear_paquete(CREAR_TABLA_SEGMENTOS);
            serializar_tabla_segmentos(tabla_segmentos, paquete);
            enviar_paquete(paquete, *socket_modulo);
            free(paquete);

            break;

        case TERMINAR:
            // recibe
            int size;
            void *buffer = recibir_buffer(&size, *socket_modulo);

            int *desplazamiento = malloc(sizeof(int));
            *desplazamiento = 0; 
            
            PID = deserializar_int(buffer, desplazamiento);
            tabla_segmentos = deserializar_tabla_segmentos(buffer, desplazamiento);

            free(desplazamiento);

            // eliminar
            printf("LISTA HUECOS ANTES DE FINALIZAR PROCESO\n");
            list_iterate(LISTA_HUECOS, (void*) imprimir_hueco);
            printf("------------------------------\n");
            finalizar_proceso(tabla_segmentos);
            printf("LISTA HUECOS DESPUES DE FINALIZAR PROCESO\n");
            list_iterate(LISTA_HUECOS, (void*) imprimir_hueco);
            printf("------------------------------\n");
            log_info(LOGGER_MEMORIA, "Eliminación de Proceso PID: <%d>", PID);

            break;

        case CREATE_SEGMENT:
            // recibe
            t_ctx* ctx = recibir_contexto(*socket_modulo);

            // crea
            paquete = crear_segmento(atoi(ctx->motivos_desalojo->parametros[0]), atoi(ctx->motivos_desalojo->parametros[1]), ctx->PID);
            // envia
            enviar_paquete(paquete, *socket_modulo);
            free(paquete);
            free(ctx);

            break;
        case DELETE_SEGMENT:
            // recibe 
            ctx = recibir_contexto(*socket_modulo);

            // elimina              
            eliminar_segmento(ctx->tabla_segmentos, atoi(ctx->motivos_desalojo->parametros[0]), ctx->PID);

            // envia
            paquete = crear_paquete(DELETE_SEGMENT);
            serializar_tabla_segmentos(ctx->tabla_segmentos, paquete);
            enviar_paquete(paquete, *socket_modulo);
            free(paquete);

            break;
        
        case COMPACTAR:
            compactar();
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
void recibir_cpu(int* socket_modulo) {
    while(1) {
        int cod_op = recibir_operacion(*socket_modulo);
        switch(cod_op) {
            
            case MOV_IN:
                t_parametros_variables* parametros = recibir_parametros_variables(*socket_modulo);
                char* valor_leido = leer_valor_direccion_fisica(strtol(parametros->parametros[1], NULL, 10));
                enviar_mensaje(valor_leido, *socket_modulo);
                free(valor_leido);
                liberar_parametros_variables(parametros);
                break;
        
            case MOV_OUT:
                parametros = recibir_parametros_variables(*socket_modulo);
                escribir_valor_direccion_fisica(parametros->parametros[0], strtol(parametros->parametros[1], NULL, 10));
                enviar_mensaje("OK", *socket_modulo);
                liberar_parametros_variables(parametros);
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

void recibir_fs(int* socket_modulo) {
    while(1) {
        int cod_op = recibir_operacion(*socket_modulo);
        switch(cod_op) {
            
            case F_READ:
                t_parametros_variables* parametros = recibir_parametros_variables(*socket_modulo);
                escribir_valor_direccion_fisica(parametros->parametros[0], strtol(parametros->parametros[1], NULL, 10));
                enviar_mensaje("OK", *socket_modulo);
                liberar_parametros_variables(parametros);

                break;

            case F_WRITE:
                parametros = recibir_parametros_variables(*socket_modulo);
                char* valor_leido = leer_valor_direccion_fisica(strtol(parametros->parametros[0], NULL, 10));
                enviar_mensaje(valor_leido, *socket_modulo);
                free(valor_leido);
                liberar_parametros_variables(parametros);

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

t_ctx* recibir_contexto(int socket)
{
    int size;
    void *buffer = recibir_buffer(&size, socket);

    int *desplazamiento = malloc(sizeof(int));
    *desplazamiento = 0; 
    
    t_ctx* ctx = deserializar_contexto(buffer, desplazamiento);
    
    free(desplazamiento);
    return ctx;
}

t_parametros_variables* recibir_parametros_variables(int socket)
{
    int size;
    void *buffer = recibir_buffer(&size, socket);

    int *desplazamiento = malloc(sizeof(int));
    *desplazamiento = 0; 
    
    t_parametros_variables* parametros = deserealizar_motivos_desalojo(buffer, desplazamiento);
    
    free(desplazamiento);
    return parametros;
}