#include <new.h>

t_list *init_new()
{
    return list_create();
}

void nuevo_proceso(t_list *estado, instrucciones *proceso){ //como recibe las instrucciones?
    list_add(estado, proceso); //falta armar pcb cuando recibe de cpu
}

t_pcb *get_proceso_desde_new(t_list *estado){ //FIFO
    return list_remove(estado, 0); //lo llama ready
}

