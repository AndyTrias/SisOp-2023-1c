#ifndef CONECTAR_CONSOLA_H
#define CONECTAR_CONSOLA_H

#include <commons/log.h>
#include <utils/mensajes.h>
#include <utils/conexiones.h>


extern t_log* logger_kernel;

void conectar_consola(int);
void nuevo_proceso(int*);

void iterator(t_instruccion*);

// Se deberia modularizar para que reciba un paquete general  
// se envie a la funcion de deserealizacion para ese paquete como parametro
t_list * recibir_paquete_consola(int);
t_instruccion* deserealizar_instruccion(void*, int* );


#endif