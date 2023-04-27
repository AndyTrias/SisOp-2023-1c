#include "new.h"

t_pcb crear_pcb(t_list* instrucciones){
  t_pcb nuevo;
  
  nuevo.PID = PID_COUNT;
  nuevo.estado_proceso = NEW;
  nuevo.instrucciones = instrucciones;
  nuevo.tiempo_desde_ult_ready = temporal_create();
  nuevo.archivos_abiertos = list_create();

  log_info(LOGGER_KERNEL, "Se Crea el proceso <%d> en NEW", PID_COUNT);

  PID_COUNT++;
  
  return nuevo;
}

void nuevo_proceso(t_list* instrucciones){ //como recibe las instrucciones?
    t_pcb proceso = crear_pcb(instrucciones);
    list_add(LISTA_NEW, &proceso); //falta armar pcb cuando recibe de cpu
}

t_pcb *get_proceso_desde_new(){ //FIFO
    return list_remove(LISTA_NEW, 0); //lo llama ready
}

