
#include "procesos.h"

int PID_count = 0;

t_pcb crear_pcb(){ //debe recibir como parametro las instrucciones
  t_pcb nuevo;
  
  nuevo.PID= PID_count;
  nuevo.estado_proceso = NEW;
  //nuevo.instrucciones = instrucciones;
  nuevo.tiempo_desde_ult_ready= temporal_create();
  nuevo.archivos_abiertos = list_create();

  PID_count++;

  return nuevo;

}