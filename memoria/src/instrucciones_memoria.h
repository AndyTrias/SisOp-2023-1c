#ifndef INSTRUCCIONES_MEMORIA_H
#define INSTRUCCIONES_MEMORIA_H

    #include <commons/collections/list.h>
    #include <utils/planificacion/pcb.h>
    #include "estructuras.h"
    #include "funciones_auxiliares.h"
    #include "globales.h"

    extern t_list* LISTA_HUECOS;
    extern t_configuracion* CONFIG;

    t_paquete* crear_segmento(int, int, int);
    void eliminar_segmento(t_list*, int, int);
    void finalizar_proceso(t_list*);
    char* leer_valor_direccion_fisica(long);
    void escribir_valor_direccion_fisica(char* , long);

#endif