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
            case MENSAJE:
                printf("Me llego un mensaje\n");
                break;
            
            case INSTRUCCIONES:
                printf("Me llego un paquete\n");
                t_list *lista = recibir_paquete_consola(*socket_consola);
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

void iterator(t_instruccion *instruccion) {
	log_info(logger_kernel, "%d", instruccion->operacion);
    for (int i = 0; i < instruccion->cantidad_parametros; i++) {
        log_info(logger_kernel, "%s", instruccion->parametros[i]);
    }
}


t_list *recibir_paquete_consola(int socket_cliente) {
    // Esto es comun en todas las funciones que reciben paquetes
    int size;
    int desplazamiento = 0;
    void * buffer;
    buffer = recibir_buffer(&size, socket_cliente);
    
    // Nuestra implementacion recibe el tamanio de la instruccion a leer
    int tamanio;


    t_list* instrucciones = list_create();

    
    while(desplazamiento < size)
    {
        // Recibe el tamanio de la instruccion
        memcpy(&tamanio, buffer + desplazamiento, sizeof(int));
        desplazamiento+=sizeof(int);
        void* instruccion = malloc(tamanio);
        
        // Recibe la instruccion serializada leyendo por tamanio
        memcpy(instruccion, buffer+desplazamiento, tamanio);
        desplazamiento+=tamanio;
        
        // La otra opcion que se me ocurre es tener una sola funcion agregar_a_paquete
        // y pasar todo el buffer pero solo leer la instruccion que me interesa
        // pasando el desplazamiento por referencia -> list_add(instrucciones, deserealizar_instruccion(buffer, &desplazamiento));
        // Con esta opcion borrariamos todo el codigo de arriba ya que el desearlizar se encarga


        list_add(instrucciones, deserealizar_instruccion(instruccion));
    }
    free(buffer);
    
    return instrucciones;
}

t_instruccion* deserealizar_instruccion(void* instruccion_serializada) {
    t_instruccion* instruccion_deserializada = malloc(sizeof(t_instruccion));
    int desplazamiento = 0;
    int tamanio_parametro;

    memcpy(&instruccion_deserializada->operacion, instruccion_serializada + desplazamiento, sizeof(t_operacion));
    desplazamiento += sizeof(t_operacion);

    memcpy(&instruccion_deserializada->cantidad_parametros, instruccion_serializada + desplazamiento, sizeof(int));
    desplazamiento += sizeof(int);

    instruccion_deserializada->parametros = malloc(sizeof(char*) * instruccion_deserializada->cantidad_parametros);
    for (int i = 0; i < instruccion_deserializada->cantidad_parametros; i++) {
        memcpy(&tamanio_parametro, instruccion_serializada + desplazamiento, sizeof(int));
        desplazamiento += sizeof(int);

        instruccion_deserializada->parametros[i] = malloc(tamanio_parametro);
        memcpy(instruccion_deserializada->parametros[i], instruccion_serializada + desplazamiento, tamanio_parametro);
        desplazamiento += tamanio_parametro;
    }

    return instruccion_deserializada;
}
