#include <ready.h>

t_list *init_ready(){
    return list_create()
}

int grado_de_multiprogramacion(t_list *ready, int procesos_en_blocked){
    return list_size(ready) + procesos_en_blocked + 1 //el 1 es el que esta en el cpu. ver como se consiguen los procesos del blocked
}

bool hay_espacio_ready(t_list *estado){
    return (GRADO_MAX_MULTIPROGRAMACION<grado_de_multiprogramacion(estado, 2)); //implementar procesos en blocked 
}

void admitir_proceso(t_list *ready, t_list *new){ //se usa una vez que se sabe que hay espacio para un proceso mas
    t_pcb *proceso = get_proceso_desde_blocked() //implementar
    
    if (proceso != NULL){ //le da prioridad a los que vienen de blocked
        list_add(ready, proceso);
    }else{
        list_add(ready, (get_proceso_desde_new(new)));
    }

    
}