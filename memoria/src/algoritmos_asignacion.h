#ifndef ALGORITMOS_ASIGNACION_H
#define ALGORITMOS_ASIGNACION_H

    #include <utils/logs.h>
    #include <utils/configs.h>
    #include <commons/collections/list.h>
    #include <utils/mensajes.h>
    #include "estructuras.h"

    t_hueco* get_hueco_con_first_fit(int);
    t_hueco* get_hueco_con_best_fit(int);
    t_hueco* get_hueco_con_worst_fit(int);
    void modificar_lista_huecos(t_hueco*, int);
    bool comprobar_compactacion(int);
#endif