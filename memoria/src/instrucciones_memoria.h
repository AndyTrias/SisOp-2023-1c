#ifndef GLOBALES_H_
#define GLOBALES_H_

    #include <commons/collections/list.h>
    #include <utils/planificacion/pcb.h>
    #include "estructuras.h"
    #include "algoritmos_asignacion.h"

    extern t_list* LISTA_HUECOS;
    extern t_configuracion* CONFIG;

    void* crear_segmento(int, int);

#endif