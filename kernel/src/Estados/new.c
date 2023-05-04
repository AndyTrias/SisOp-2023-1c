#include "new.h"

t_pcb crear_pcb(t_list* instrucciones){
  t_pcb nuevo;
  
  nuevo.contexto.PID = PID_COUNT;
  nuevo.contexto.program_counter = 0;
  nuevo.contexto.instrucciones = instrucciones;
  
  nuevo.tiempo_desde_ult_ready = temporal_create();
  nuevo.archivos_abiertos = list_create();

  log_info(LOGGER_KERNEL, "Se Crea el proceso <%d> en NEW", PID_COUNT);

  PID_COUNT++;
  
  return nuevo;
}

void nuevo_proceso(t_list* instrucciones){ //como recibe las instrucciones?
    t_pcb proceso = crear_pcb(instrucciones);
    pthread_mutex_lock(&MUTEX_LISTA_NEW);
    list_add(LISTA_NEW, &proceso);
    pthread_mutex_unlock(&MUTEX_LISTA_NEW);
    agregar_a_ready_si_hay_espacio();
}

// Es llamada por ready cada vez que necesita agregar un proceso a Ready porque tiene espacio
// FIFO
t_pcb *get_proceso_desde_new(){ 
    return list_remove(LISTA_NEW, 0); 
}

