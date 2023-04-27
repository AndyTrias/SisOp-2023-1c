
#include "procesos.h"

t_pcb crear_pcb(){ //debe recibir como parametro las instrucciones
  t_pcb nuevo;
  
  nuevo.PID= PID_count;
  nuevo.estado_proceso = NEW;
  //nuevo.instrucciones = instrucciones;
  nuevo.tiempo_desde_ult_ready= temporal_create();
  nuevo.archivos_abiertos = list_create();

  log_info(logger_kernel, "Se Crea el porceso <%d> en NEW", PID_count);
  
  PID_count++;
  

  return nuevo;

}