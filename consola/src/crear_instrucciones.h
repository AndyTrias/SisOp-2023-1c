#ifndef CREAR_INSTRUCCIONES_H
#define CREAR_INSTRUCCIONES_H

    #include <utils/mensajes.h>
    extern t_log* logger_consola;

    t_operacion obtener_instrucciones(char *);
    int obtener_cantidad_parametros(t_operacion op);
    t_instruccion* crear_estructura_instruccion(char);
    int serializar_instruccion(t_instruccion*, char*); 
    void agregar_a_paquete(t_paquete*, void*, int);

#endif