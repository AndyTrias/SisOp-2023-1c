
#ifndef INSTRUCCIONESCPU_H_
#define INSTRUCCIONESCPU_H_

#include <commons/log.h>
#include <utils/conexiones.h>
#include <utils/planificacion/pcb.h>
#include <utils/mensajes.h>
#include <unistd.h>

// #include <math.h>
extern t_log* LOGGER_CPU;
void ciclo_de_instruccion(t_ctx* ctx);

#endif