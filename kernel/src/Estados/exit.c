#include "exit.h"

void terminar_proceso(t_pcb *proceso){
    //limpiar memoria
    
    t_paquete* paquete = crear_paquete(TERMINAR);
    enviar_paquete(paquete, proceso->socket_consola);
    free(paquete);
    
    sem_post(&GRADO_MULTIPROGRAMACION);
}