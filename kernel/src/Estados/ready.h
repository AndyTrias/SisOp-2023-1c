#ifndef READY_H_
#define READY_H_

    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>
    #include <commons/collections/list.h>
    #include "new.h"

    int get_grado_de_multiprogramacion(int);
    bool hay_espacio_ready();
    void admitir_proceso();
    t_pcb *get_proceso_por_fifo();
    t_pcb *get_proceso_por_hrrn();
    t_pcb *ceder_proceso_a_cpu();

#endif