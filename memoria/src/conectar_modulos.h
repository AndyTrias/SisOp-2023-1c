#ifndef CONECTAR_MODULOS_H
#define CONECTAR_MODULOS_H

    #include <commons/log.h>
    #include <pthread.h>
    #include <utils/conexiones.h>
    #include <utils/mensajes.h>

    extern t_log* logger_memoria;

    void conectar_modulos(int);
    void nuevo_modulo(int*);
    void iterator(char*);

#endif