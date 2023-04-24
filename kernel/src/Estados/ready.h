#include <stdio.h>
#include <stdlib.h>
#include <commons/collections/list.h>
#include <new.h>

t_list *init_ready();

bool hay_espacio_ready(t_list *estado);

int grado_de_multiprogramacion(t_list *ready, int procesos_en_blocked);

void admitir_proceso(t_list *ready, t_list *new);

t_pcb *ceder_proceso_a_cpu(t_list *ready);

t_pcb *FIFO(t_list *ready);

t_pcb *HRRN(t_list *ready);
