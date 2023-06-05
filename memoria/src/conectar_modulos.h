#ifndef CONECTAR_MODULOS_H
#define CONECTAR_MODULOS_H

    #include <commons/log.h>
    #include <pthread.h>
    #include <utils/conexiones.h>
    #include <utils/mensajes.h>

    extern t_log* LOGGER_MEMORIA;

    void conectar_modulos(int);
    void nuevo_modulo(int*);

#endif