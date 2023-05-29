#include "new.h"

t_pcb *crear_pcb(t_list* instrucciones, int socket_consola){
  t_pcb *nuevo = malloc(sizeof(t_pcb));
  
  nuevo->contexto.PID = PID_COUNT;
  nuevo->contexto.program_counter = 0;
  nuevo->contexto.cant_instrucciones = list_size(instrucciones);
  nuevo->contexto.instrucciones = instrucciones;
  nuevo->estimado_prox_rafaga= ESTIMACION_INICIAL;
  
  nuevo->contexto.motivos_desalojo = malloc(sizeof(t_parametros_variables));
  nuevo->contexto.motivos_desalojo->cantidad_parametros = 0;
  nuevo->contexto.motivos_desalojo->parametros = NULL;
  
  nuevo->tiempo_desde_ult_ready = temporal_create();
  nuevo->archivos_abiertos = list_create();

  nuevo->socket_consola = socket_consola;

  log_info(LOGGER_KERNEL, "Se Crea el proceso <%d> en NEW", PID_COUNT);

  PID_COUNT++;
  
  return nuevo;
}

void nuevo_proceso(t_list* instrucciones, int socket_consola){ 
    t_pcb *proceso = crear_pcb(instrucciones, socket_consola);
    
    agregar_a_lista_new(proceso);
    
}

// Es llamada por ready cada vez que necesita agregar un proceso a Ready porque tiene espacio
// FIFO
t_pcb *get_proceso_desde_new(){ 
    return sacar_de_lista_new(0); 
}

