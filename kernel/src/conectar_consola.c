#include "conectar_consola.h"

void conectar_consola(int socket_servidor) {
    log_info(logger_kernel, "Esperando conexiones de consolas...");
    pthread_t hilo_consola;

    while (1) {
        int socket_consola = esperar_cliente(socket_servidor);
        log_info(logger_kernel, "Se conecto una consola");
        pthread_create(&hilo_consola, NULL, (void*) nuevo_proceso, &socket_consola);
        pthread_detach(hilo_consola);
    }
}

void nuevo_proceso(int* socket_consola) {
    while(1) {
        int cod_op = recibir_operacion(*socket_consola);
        switch(cod_op) {
            case INSTRUCCION:
                recibir_paquete(*socket_consola);
                break;
            
            case MENSAJE:
                recibir_mensaje(*socket_consola);
                break;
            
            case PAQUETE:
                t_list* lista = recibir_paquete(*socket_consola);
                log_info(logger_kernel, "Me llegaron los siguientes valores:\n");
                list_iterate(lista, (void*) iterator);
                break;
            
            case -1:
                log_info(logger_kernel, "Se desconecto una consola");
                return;
            
            default:
                log_error(logger_kernel, "Operacion desconocida");
                return;
        }
    }
}

void iterator(char* value) {
	log_info(logger_kernel,"%s", value);
}