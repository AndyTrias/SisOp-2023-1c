#ifndef PROCESOS_H_
#define PROCESOS_H_

#include <utils/planificacion/pcb.h>
#include "globales.h"

extern t_log* logger_kernel;
extern int PID_count;
t_pcb crear_pcb();

#endif