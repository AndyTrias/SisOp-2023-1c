#include "conectar_consola.h"

void conectar_consola(int socket_servidor)
{
    pthread_t hilo_consola;

    while (1)
    {
        int socket_consola = esperar_cliente(socket_servidor);
        log_info(LOGGER_KERNEL, "Se conecto una consola");
        pthread_create(&hilo_consola, NULL, (void *) enviado_de_consola, &socket_consola);
        pthread_detach(hilo_consola);
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
            printf("Me llego un mensaje\n");
            break;

        case INSTRUCCIONES:
            printf("Me llego un paquete\n");
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

t_instruccion *deserealizar_instruccion(void *buffer, int *desplazamiento)
{
    t_instruccion *instruccion_deserializada = malloc(sizeof(t_instruccion));
    int tamanio_parametro;

    memcpy(&instruccion_deserializada->operacion, buffer + *desplazamiento, sizeof(t_operacion));
    *desplazamiento += sizeof(t_operacion);

    memcpy(&instruccion_deserializada->cantidad_parametros, buffer + *desplazamiento, sizeof(int));
    *desplazamiento += sizeof(int);

    instruccion_deserializada->parametros = malloc(sizeof(char *) * instruccion_deserializada->cantidad_parametros);
    for (int i = 0; i < instruccion_deserializada->cantidad_parametros; i++)
    {
        memcpy(&tamanio_parametro, buffer + *desplazamiento, sizeof(int));
        *desplazamiento += sizeof(int);

        instruccion_deserializada->parametros[i] = malloc(tamanio_parametro);
        memcpy(instruccion_deserializada->parametros[i], buffer + *desplazamiento, tamanio_parametro);
        *desplazamiento += tamanio_parametro;
    }
    
    return instruccion_deserializada;
}
