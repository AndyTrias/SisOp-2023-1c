#include "exit.h"

void liberar_recursos(t_pcb *proceso)
{
    if (!list_is_empty(proceso->recursos_en_uso))
    {
        int i;
        char *aux;
        for (i = 0; i < list_size(proceso->recursos_en_uso); i++)
        {
            aux = list_get(proceso->recursos_en_uso, i);
            signal(proceso, aux);
        }
    }
}

void terminar_proceso(t_pcb *proceso)
{
    pthread_mutex_lock(&SOLICITUD_MEMORIA);

    t_paquete *paquete = crear_paquete(TERMINAR);
    agregar_a_paquete_dato_serializado(paquete, &proceso->contexto->PID, sizeof(int));
    serializar_tabla_segmentos(proceso->contexto->tabla_segmentos, paquete);
    enviar_paquete(paquete, SOCKET_MEMORIA);
    eliminar_paquete(paquete);
    recibir_operacion(SOCKET_MEMORIA);
    recibir_mensaje(SOCKET_MEMORIA);

    pthread_mutex_unlock(&SOLICITUD_MEMORIA);

    // Revisar si tiene un recurso asignado y eliminarlo
    liberar_recursos(proceso);

    // destruir proceso
    list_destroy_and_destroy_elements(proceso->contexto->tabla_segmentos, (void*) liberar_segmento);
    liberar_contexto(proceso->contexto);

    
    paquete = crear_paquete(TERMINAR);
    enviar_paquete(paquete, proceso->socket_consola);
    eliminar_paquete(paquete);

    // liberar_lista_tabla_segmentos(proceso->contexto->tabla_segmentos);

    // free(proceso);
    
    sem_post(&GRADO_MULTIPROGRAMACION);
    reemplazar_exec_por_nuevo();
}

// void liberar_lista_tabla_segmentos(t_list *lista_tabla_segmentos)
// {
//     // Itera sobre la lista de tabla de segmentos

//     // Libera la lista de tabla de segmentos y sus elementos
//     list_destroy_and_destroy_elements(lista_tabla_segmentos, liberar_tabla_segmentos);
// }

// void liberar_tabla_segmentos(void *elemento)
// {
//     t_tabla_segmentos *tabla_segmentos = (t_tabla_segmentos *)elemento;

//     // Libera la lista de segmentos de la tabla
//     for (int i = 0; i< list_size(tabla_segmentos->segmentos); i++)
//     {
//         t_segmento *segmento = list_get(tabla_segmentos->segmentos, i);
//         free(segmento->base);
//         free(segmento);
//     }

//     // Libera la estructura de la tabla de segmentos
//     free(tabla_segmentos);
// }

// void liberar_lista_instrucciones(t_list *lista_instrucciones)
// {
//     // Itera sobre la lista de instrucciones
//     void liberar_instruccion(void *elemento)
//     {
//         t_instruccion *instruccion = (t_instruccion *)elemento;

//         // Libera los parámetros de la instrucción
//         for (int i = 0; i < instruccion->cantidad_parametros; i++)
//         {
//             free(instruccion->parametros[i]);
//         }
//         free(instruccion->parametros);

//         // Libera la estructura de la instrucción
//         free(instruccion);
//     }

//     // Libera la lista de instrucciones y sus elementos
//     list_destroy_and_destroy_elements(lista_instrucciones, liberar_instruccion);
// }