#include "manejo_memoria.h"

void crear_segmento(t_pcb *proceso){// enviar a memoria CREATE_SEGMENT con sus 2 parametros (id del segmento y tamanio)
    //se solicita la creacion del segmento
    t_paquete *paquete = crear_paquete(CREATE_SEGMENT);
    serializar_contexto(&proceso->contexto, paquete);
    enviar_paquete(paquete, SOCKET_MEMORIA);
    free(paquete);

    int cod_op = recibir_operacion(SOCKET_MEMORIA);
    int size;
    void* buffer = recibir_buffer(&size, SOCKET_MEMORIA);
    
    switch(cod_op){
        case CREATE_SEGMENT:
            t_segmento* segmento = list_get(proceso->contexto.tabla_segmentos, atoi(proceso->contexto.motivos_desalojo->parametros[0]));

            memcpy(&(segmento->base), buffer, sizeof(void*));
            segmento->tamanio = atoi(proceso->contexto.motivos_desalojo->parametros[1]);

            log_info(LOGGER_KERNEL, "PID: <%d> - Crear Segmento - Id: <%d> - Tamaño: <%d>", proceso->contexto.PID, segmento->id_segmento, segmento->tamanio);
            //supongo que cuando termina de crear el segmento se manda de nuevo al cpu por ende no cambio de pcb
            break;
        case COMPACTAR:
            /*t_paquete *paquete = crear_paquete(COMPACTAR);
            serializar_contexto(&proceso->contexto, paquete);
            enviar_paquete(paquete, SOCKET_MEMORIA);
            free(paquete);
            */
            break;
        case OUT_OF_MEMORY:
            log_info(LOGGER_KERNEL,"Finaliza el proceso <%d> - Motivo: <OUT_OF_MEMORY>",proceso->contexto.PID);
            terminar_proceso(proceso);
            break;

    }
    
}

void eliminar_segmento(t_pcb *proceso){
    // enviar a memoria DELETE_SEGMENT con su parametro (id del segmento)

    t_paquete *paquete = crear_paquete(DELETE_SEGMENT);
    serializar_contexto(&proceso->contexto, paquete);
    enviar_paquete(paquete, SOCKET_MEMORIA);
    free(paquete);

    recibir_operacion(SOCKET_MEMORIA);
    t_list* tabla_segmentos_actualizada = recibir_tabla_segmentos(SOCKET_MEMORIA);

    proceso->contexto.tabla_segmentos = tabla_segmentos_actualizada;

    log_info(LOGGER_KERNEL, "PID: <%d> - Eliminar Segmento - Id Segmento: <%d>", proceso->contexto.PID, atoi(proceso->contexto.motivos_desalojo->parametros[0]));
}