#ifndef EXEC_H_
#define EXEC_H_

    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>
    #include "ready.h"
    #include <commons/collections/list.h>
    #include <utils/mensajes.h>
    #include "exit.h"

    void recibir_de_cpu();
    void empezar_ciclo_si_vacio();
    void enviar_a_cpu();
    void mandar_a_exit_o_blocked(t_pcb *);
    void reemplazar_ctx(t_ctx *);
    t_pcb* reemplazar_proceso(t_pcb *);




#endif