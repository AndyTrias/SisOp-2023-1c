#ifndef CONECTAR_MODULOS_H
#define CONECTAR_MODULOS_H

    #include <commons/log.h>
    #include <pthread.h>
    #include <utils/conexiones.h>
    #include <utils/mensajes.h>
    #include <utils/planificacion/pcb.h>

    extern t_log* LOGGER_MEMORIA;

    void conectar_modulos(int);
    void nuevo_modulo(int*);
    void recibir_kernel(int*);
    t_ctx* recibir_paquete_kernel(int);

#endif