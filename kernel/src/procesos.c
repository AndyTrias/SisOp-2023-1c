
#include "procesos.h"

t_pcb crear_pcb(int *PID_count){ //debe recibir como parametro las instrucciones
  t_pcb nuevo;
  
  nuevo.PID= *PID_count;
  nuevo.estado_proceso = NEW;
  //nuevo.instrucciones = instrucciones;
  nuevo.tiempo_desde_ult_ready= temporal_create();
  nuevo.archivos_abiertos = list_create();

  *PID_count = *PID_count + 1; 

  return nuevo;

}