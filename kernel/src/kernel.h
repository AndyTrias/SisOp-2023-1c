#ifndef KERNEL_H
#define KERNEL_H

#include <utils/logs.h>
#include <utils/aux.h>
#include <utils/configs.h>
#include <utils/conexiones.h>
#include <init.h>
#include <stdio.h>
#include <stdlib.h>
#include <utils/mensajes.h>
#include <utils/planificacion/pcb.h>

t_pcb crear_pcb(int );
void incrementar(int *,int );
#endif