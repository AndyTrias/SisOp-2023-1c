#include "exit.h"

void terminar_proceso(t_pcb *proceso){
    //limpiar memoria
    sem_post(&GRADO_MULTIPROGRAMACION);
}