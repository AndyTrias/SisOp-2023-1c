#ifndef EXEC_H_
#define EXEC_H_

    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>
    #include "ready.h"
    #include <commons/collections/list.h>
    #include <utils/mensajes.h>
    #include "exit.h"

    void reemplazar_proceso(t_pcb *);
    void reemplazar_ctx(t_ctx *);
    void mandar_a_exit_o_blocked(t_pcb *);
    void enviar_a_cpu();
    void empezar_ciclo_si_vacio();
    void definir_accion(int, t_pcb *);
    void recibir_de_cpu(int);
    void reemplazar_exec_por_nuevo();







#endif