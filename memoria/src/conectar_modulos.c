#include "conectar_modulos.h"

void conectar_modulos(int socket_servidor) {
    log_info(logger_memoria, "Esperando conexiones de modulos...");
    
    pthread_t hilo_kernel, hilo_cpu, hilo_fs;
    int socket_kernel, socket_cpu, socket_fs, socket_cliente;

    for (int i = 0; i < 3; i++) {
        socket_cliente = esperar_cliente(socket_servidor);
        switch(recibir_operacion(socket_cliente)) {
            case 0:
                log_info(logger_memoria, "Se conecto el file system");
                socket_fs = socket_cliente;
                pthread_create(&hilo_fs, NULL, (void *) nuevo_modulo, &socket_fs);
                pthread_detach(hilo_fs);
                break;
            case 1:
                log_info(logger_memoria, "Se conecto el kernel");
                socket_kernel = socket_cliente;
                pthread_create(&hilo_kernel, NULL, (void *) nuevo_modulo, &socket_kernel);
                pthread_join(hilo_kernel, NULL);
                break;
            case 2:
                log_info(logger_memoria, "Se conecto el cpu");
                socket_cpu = socket_cliente;
                pthread_create(&hilo_cpu, NULL, (void *) nuevo_modulo, &socket_cpu);
                pthread_detach(hilo_cpu);
                break;
        }
    }

}

void nuevo_modulo(int* socket_modulo) {
    while(1) {
        int cod_op = recibir_operacion(*socket_modulo);
        switch(cod_op) {
            
            case MENSAJE:
                char* mensaje_recibido = recibir_mensaje(*socket_modulo);
                log_info(logger_memoria, "Me llego el mensaje: %s", mensaje_recibido);
                free(mensaje_recibido);
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