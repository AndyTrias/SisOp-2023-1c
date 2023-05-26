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

void definir_accion(int cod_op, t_pcb *proceso){
    switch (cod_op)
    {
    case DESALOJAR:
        agregar_a_lista_ready(proceso);
        reemplazar_exec_por_nuevo();
        log_info(LOGGER_KERNEL, "Se recibio un mensaje de desalojo");
        break;

    case PETICION:
        reemplazar_exec_por_nuevo();
        log_info(LOGGER_KERNEL, "Se recibio un mensaje de peticion");
        //I/O
        break;
    
    case BLOQUEAR:
        reemplazar_exec_por_nuevo();
        log_info(LOGGER_KERNEL, "Se recibio un mensaje de bloqueo");
        break;

    case TERMINAR:
        reemplazar_exec_por_nuevo();
        log_info(LOGGER_KERNEL, "Se recibio un mensaje de finalizacion");
        terminar_proceso(proceso);
        break;

    case WAIT:
        if ( 1 /*no hay recursos disponibles*/){
            reemplazar_exec_por_nuevo();
            log_info(LOGGER_KERNEL, "Se recibio un mensaje de wait");
        }
        else{
            //se le da el recurso;
        };
        break;


    }
}

void reemplazar_exec_por_nuevo(){
    t_pcb *proceso_entrante = ceder_proceso_a_exec(); //pide un proceso a ready segun el algoritmo

    reemplazar_proceso(proceso_entrante);
}

void recibir_de_cpu(int conexion_cpu){
        
    int cod_op = recibir_operacion(conexion_cpu);
    
    int size;
    void* buffer = recibir_buffer(&size, conexion_cpu);
    
    t_ctx * ctx = deserializar_contexto(buffer);

    reemplazar_ctx(ctx);

    definir_accion(cod_op, EJECUTANDO);

    enviar_a_cpu();


}