#include "exit.h"

void terminar_proceso(t_pcb *proceso){
    
    list_destroy(proceso->contexto.instrucciones);
    list_destroy(proceso->archivos_abiertos);
    temporal_destroy(proceso->tiempo_desde_ult_ready);
    
    
    t_paquete* paquete = crear_paquete(TERMINAR);
    enviar_paquete(paquete, proceso->socket_consola);
    free(paquete);
    
    sem_post(&GRADO_MULTIPROGRAMACION);
}