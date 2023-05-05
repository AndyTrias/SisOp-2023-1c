#ifndef CONECTAR_CONSOLA_H
#define CONECTAR_CONSOLA_H

#include <commons/log.h>
#include <utils/mensajes.h>
#include <utils/conexiones.h>
#include <Estados/new.h>

extern t_log* LOGGER_KERNEL;

void conectar_consola(int);
void enviado_de_consola(int*);

void iterator(t_instruccion*);

// Se deberia modularizar para que reciba un paquete general  
// se envie a la funcion de deserealizacion para ese paquete como parametro
t_list * recibir_paquete_consola(int);


#endif