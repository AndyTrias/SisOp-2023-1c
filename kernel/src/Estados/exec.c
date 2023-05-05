#include "exec.h"

t_pcb *ejecutando;

t_pcb* reemplazar_proceso(t_pcb *nuevo_pcb){
    t_pcb *aux = ejecutando;
    ejecutando = nuevo_pcb;
    return aux;
}

void reemplazar_ctx(t_ctx *nuevo_ctx){
    ejecutando->contexto = *nuevo_ctx;
}

void mandar_a_exit_o_blocked(t_pcb *proceso){
    if(list_size(proceso->contexto.instrucciones)==0){ //transitorio
        terminar_proceso(proceso);
    }else{
        //mandar_a_blocked(proceso);
    }
}

void enviar_a_cpu(){
    
    t_paquete *paquete = crear_paquete(CONTEXTO);
    
    serializar_contexto(&ejecutando->contexto, paquete);

    enviar_paquete(paquete, SOCKET_CPU);
}



void empezar_ciclo_si_vacio(){
    if (ejecutando == NULL){ 
        ejecutando = ceder_proceso_a_exec();
        enviar_a_cpu();
    }
    
}

//  TODO: Recibe el paquete con el contexto
void recibir_de_cpu(int conexion_cpu){
        
    int cod_op = recibir_operacion(conexion_cpu);
    
    int size;
    void* buffer = recibir_buffer(&size, conexion_cpu);
    
    t_ctx * ctx = deserializar_contexto(buffer);
    reemplazar_ctx(ctx);
    
    switch (cod_op)
    {
    case DESALOJAR:
        log_info(LOGGER_KERNEL, "Se recibio un mensaje de desalojo");
        break;
    
    case BLOQUEAR:
        log_info(LOGGER_KERNEL, "Se recibio un mensaje de bloqueo");
        break;

    case TERMINAR:
        log_info(LOGGER_KERNEL, "Se recibio un mensaje de finalizacion");
        break;


    default:
        log_error(LOGGER_KERNEL, "Operacion desconocida");
        return;
    }

    //recibe de cpu
    
    // Le pide  un proceso a ready segun su algoritmo
    // t_pcb *proceso_entrante = ceder_proceso_a_exec();
    // t_pcb *proceso_saliente = reemplazar_proceso(proceso_entrante);
    // enviar_a_cpu();
    // mandar_a_exit_o_blocked(proceso_saliente);

}