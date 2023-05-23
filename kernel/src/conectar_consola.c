#include "conectar_consola.h"

void conectar_consola(int socket_servidor)
{
    log_info(LOGGER_KERNEL, "Esperando conexiones de consolas...");
    pthread_t hilo_consola;

    while (1)
    {
        int socket_consola = esperar_cliente(socket_servidor);
        log_info(LOGGER_KERNEL, "Se conecto una consola");
        pthread_create(&hilo_consola, NULL, (void *) enviado_de_consola, &socket_consola);
        pthread_detach(hilo_consola);
        enviado_de_consola(&socket_consola);
    }
}

void enviado_de_consola(int *socket_consola)
{
    while (1)
    {
        int cod_op = recibir_operacion(*socket_consola);
        switch (cod_op)
        {
        case MENSAJE:
            log_info(LOGGER_KERNEL, "Se recibio un mensaje de una consola");
            
            break;

        case INSTRUCCIONES:
            log_info(LOGGER_KERNEL, "Se recibieron instrucciones de una consola");
            t_list *instrucciones = recibir_paquete_consola(*socket_consola);
            nuevo_proceso(instrucciones);
            break;

        case -1:
            log_info(LOGGER_KERNEL, "Se desconecto una consola");
            return;

        default:
            log_error(LOGGER_KERNEL, "Operacion desconocida");
            return;
        }
    }
    
}

void iterator(t_instruccion *instruccion)
{
    log_info(LOGGER_KERNEL, "%d", instruccion->operacion);
    for (int i = 0; i < instruccion->cantidad_parametros; i++)
    {
        log_info(LOGGER_KERNEL, "%s", instruccion->parametros[i]);
    }
}

t_list *recibir_paquete_consola(int socket_cliente)
{
    int size;
    void *buffer = recibir_buffer(&size, socket_cliente);

    // Nuestra implementacion recibe el tamanio de la instruccion a leer
    int desplazamiento = 0;

    t_list *instrucciones = list_create();

    while (desplazamiento < size)
    {
        list_add(instrucciones, deserealizar_instruccion(buffer, &desplazamiento));
    }
    free(buffer);

    return instrucciones;
}