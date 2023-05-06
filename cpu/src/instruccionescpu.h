
#ifndef INSTRUCCIONESCPU_H_
#define INSTRUCCIONESCPU_H_

#include <commons/log.h>
#include <utils/conexiones.h>
#include <utils/planificacion/pcb.h>
#include <utils/mensajes.h>
#include <unistd.h>
#include <commons/string.h>
#include "globales.h"


extern t_log* LOGGER_CPU;
extern int TIEMPO_RETARDO;

void ciclo_de_instruccion(t_ctx* ctx);

#endif