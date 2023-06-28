#include "exit.h"

void terminar_proceso(t_pcb *proceso){
    // Revisar si tiene un recurso asignado y eliminarlo
    if (!list_is_empty(proceso->recursos_en_uso)){
        int i;
        for (i = 0; i < list_size(proceso->recursos_en_uso); i++)
        {
            signal(proceso,list_remove(proceso->recursos_en_uso,0));
        }
    }
    // destruir proceso
    list_destroy(proceso->contexto.instrucciones);
    list_destroy(proceso->archivos_abiertos);
    list_destroy(proceso->recursos_en_uso);
    temporal_destroy(proceso->tiempo_desde_ult_ready);
    
    t_paquete* paquete = crear_paquete(TERMINAR);
    enviar_paquete(paquete, proceso->socket_consola);

    serializar_contexto(&proceso->contexto, paquete);
    enviar_paquete(paquete, SOCKET_MEMORIA);
    free(paquete);
    
    sem_post(&GRADO_MULTIPROGRAMACION);
    EJECUTANDO= ceder_proceso_a_exec();
}