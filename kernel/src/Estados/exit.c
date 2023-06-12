#include "exit.h"

void terminar_proceso(t_pcb *proceso){
    
    list_destroy(proceso->contexto.instrucciones);
    list_destroy(proceso->archivos_abiertos);
    temporal_destroy(proceso->tiempo_desde_ult_ready);
    
    //enviar_mensaje("Libera las estructuras de memoria", SOCKET_MEMORIA);
    free(proceso->contexto.tabla_segmentos);
    
    t_paquete* paquete = crear_paquete(TERMINAR);
    enviar_paquete(paquete, proceso->socket_consola);
    free(paquete);
    
    sem_post(&GRADO_MULTIPROGRAMACION);
    EJECUTANDO= ceder_proceso_a_exec();
}