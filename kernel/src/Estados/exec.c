#include "exec.h"
/*
t_pcb *ejecutando;

t_pcb* reemplazar_proceso(t_pcb *nuevo_pcb){
    t_pcb *aux = ejecutando;
    ejecutando = nuevo_pcb;
    return aux;
}

void reemplazar_ctx(t_ctx *nuevo_ctx){
    ejecutando->contexto = nuevo_ctx;
}

void mandar_a_exit_o_blocked(t_pcb *proceso){
    if(list_size(proceso->contexto.instrucciones)==0){ //transitorio
        terminar_proceso(proceso);
    }else{
        mandar_a_blocked(proceso);
    }
}

void enviar_a_cpu(){
    //envia a cpu el procey desso en ejecutando
    // Suponemos que existe una variable SOCKET_CPU
    // No sabemos como a llegar -> Metemos un patron adapter
    t_paquete *paquete = crear_paquete(CONTEXTO);
    
    serializar_contexto(ejecutando->contexto, paquete);
    
    enviar_paquete(paquete, SOCKET_CPU);
}



void empezar_ciclo_si_vacio(){
    if (ejecutando == NULL){ 
        ejecutando = ceder_proceso_a_exec();
        enviar_a_cpu();
    }
    
}

void recibir_de_cpu(){

    t_ctx *ctx;
    //recibe de cpu
    reemplazar_ctx(ctx);
    t_pcb *proceso_entrante = ceder_proceso_a_exec();
    t_pcb *proceso_saliente = reemplazar_proceso(proceso_entrante);
    enviar_a_cpu();
    mandar_a_exit_o_blocked(proceso_saliente):

}

*/

