#include "conectar_modulos.h"

void conectar_modulos(int socket_servidor) {
    log_info(logger_memoria, "Esperando conexiones de modulos...");
    
    pthread_t hilo_modulo;

    while (1) {
        int socket_modulo = esperar_cliente(socket_servidor);
        log_info(logger_memoria, "Se conecto un modulo");
        pthread_create(&hilo_modulo, NULL, (void*) nuevo_modulo, &socket_modulo);
        pthread_detach(hilo_modulo);
    }
}

void nuevo_modulo(int* socket_modulo) {
    while(1) {
        int cod_op = recibir_operacion(*socket_modulo);
        switch(cod_op) {
            case INSTRUCCIONES_CONSOLA:
                recibir_paquete(*socket_modulo);
                break;
            
            case MENSAJE:
                recibir_mensaje(*socket_modulo);
                break;
            
            case PAQUETE:
                t_list* lista = recibir_paquete(*socket_modulo);
                log_info(logger_memoria, "Me llegaron los siguientes valores:\n");
                list_iterate(lista, (void*) iterator);
                break;
            
            case -1:
                log_info(logger_memoria, "Se desconecto un modulo");
                return;
            
            default:
                log_error(logger_memoria, "Operacion desconocida");
                return;
        }
    }
}

void iterator(char* value) {
    log_info(logger_memoria,"%s", value);
}