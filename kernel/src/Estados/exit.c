#include "exit.h"

void terminar_proceso(t_pcb *proceso){
    // Revisar si tiene un recurso asignado y eliminarlo
    if (!list_is_empty(proceso->recursos_en_uso)){
        int i;
        for (i = 0; i < list_size(proceso->recursos_en_uso); i++)
        {
            signal(proceso,list_get(proceso->recursos_en_uso,0));
        }
    }
    // destruir proceso
    list_destroy(proceso->contexto.instrucciones);
    list_destroy(proceso->archivos_abiertos);
    list_destroy(proceso->recursos_en_uso);

    t_paquete* paquete = crear_paquete(TERMINAR);
    agregar_a_paquete_dato_serializado(paquete, &proceso->contexto.PID, sizeof(int));
    serializar_tabla_segmentos(proceso->contexto.tabla_segmentos, paquete);
    enviar_paquete(paquete, SOCKET_MEMORIA);
    free(paquete);

    paquete = crear_paquete(TERMINAR);
    enviar_paquete(paquete, proceso->socket_consola);
    free(paquete);
    
    sem_post(&GRADO_MULTIPROGRAMACION);
    EJECUTANDO= ceder_proceso_a_exec();
}