#ifndef INIT_KERNEL_H
#define INIT_KERNEL_H

    #include <utils/conexiones.h>
    #include <utils/configs.h>
    #include "globales.h"

    void inicializar_conexiones(int *, int *, int *, t_config*);
    void inicializar_variables_globales(t_config*);
    void inicializar_semaforos();

    void inicializar_semaforos_recursos();
    void inicializar_listas_blocked();
#endif