#include "exec.h"

t_pcb* reemplazar_proceso(t_pcb *nuevo_pcb){
    t_pcb *aux = EJECUTANDO;
    EJECUTANDO = nuevo_pcb;
    return aux;
}

void reemplazar_ctx(t_ctx *nuevo_ctx){
    EJECUTANDO->contexto = *nuevo_ctx;
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
    
    serializar_contexto(&EJECUTANDO->contexto, paquete);

    enviar_paquete(paquete, SOCKET_CPU);
}



void empezar_ciclo_si_vacio(){
    if (EJECUTANDO == NULL){ 
        EJECUTANDO = ceder_proceso_a_exec();
        enviar_a_cpu();
    }
    
}

definir_accion(int cod_op, t_pcb *proceso){
    switch (cod_op)
    {
    case DESALOJAR:
        agregar_a_lista_ready(proceso);
        log_info(LOGGER_KERNEL, "Se recibio un mensaje de desalojo");
        break;

    case PETICION:
        log_info(LOGGER_KERNEL, "Se recibio un mensaje de peticion");
        //I/O
        break;
    
    case BLOQUEAR:
        log_info(LOGGER_KERNEL, "Se recibio un mensaje de bloqueo");
        break;

    case TERMINAR:
        terminar_proceso(proceso);
        log_info(LOGGER_KERNEL, "Se recibio un mensaje de finalizacion");
        break;

    }
}

//  TODO: Recibe el paquete con el contexto
void recibir_de_cpu(int conexion_cpu){
        
    int cod_op = recibir_operacion(conexion_cpu);
    
    int size;
    void* buffer = recibir_buffer(&size, conexion_cpu);
    
    t_ctx * ctx = deserializar_contexto(buffer);

    reemplazar_ctx(ctx);

    definir_accion(cod_op, EJECUTANDO);

    t_pcb *proceso_entrante = ceder_proceso_a_exec(); //pide un proceso a ready segun el algoritmo

    enviar_a_cpu();


}