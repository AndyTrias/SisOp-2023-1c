#ifndef CONEXIONES_MODULOS_H
#define CONEXIONES_MODULOS_H

#include "utils/planificacion/pcb.h"
#include <utils/mensajes.h>
#include <utils/conexiones.h>
#include <solicitudes.h>

extern t_log* LOGGER_FILE_SYSTEM;


void conectar_kernel(int);
void enviado_de_kernel(int *);
t_parametros_variables* recibir_paquete_kernel(int);
char* leer_direccion_de_memoria(char *);
void escribir_valor_en_memoria(char *, char *);


#endif