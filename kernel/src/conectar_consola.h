#ifndef CONECTAR_CONSOLA_H
#define CONECTAR_CONSOLA_H

#include <commons/log.h>
#include <utils/mensajes.h>
#include <utils/conexiones.h>

extern t_log* logger_kernel;

void conectar_consola(int);
void nuevo_proceso(int*);

#endif