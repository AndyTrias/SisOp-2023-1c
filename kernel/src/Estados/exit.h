#ifndef EXIT_H_
#define EXIT_H_

    #include <stdio.h>
    #include <stdlib.h>
    #include <commons/collections/list.h>
    #include <manejo_fs.h>
    #include "exec.h"
    #include "ready.h"
    #include "blocked.h"
    #include <utils/mensajes.h>

    void terminar_proceso(t_pcb *);
    void liberar_recursos(t_pcb * );
    void liberar_lista_tabla_segmentos(t_list*);
    void liberar_tabla_segmentos(void*);
    void liberar_lista_instrucciones(t_list *lista_instrucciones);
    void liberar_instruccion(void *elemento);







#endif