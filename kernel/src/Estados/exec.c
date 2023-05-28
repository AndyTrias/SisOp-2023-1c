#include "exec.h"

void reemplazar_proceso(t_pcb *nuevo_pcb){
    EJECUTANDO = nuevo_pcb;
}

void reemplazar_ctx(t_ctx *nuevo_ctx){
    EJECUTANDO->contexto = *nuevo_ctx;
}



void enviar_a_cpu(){
    
    t_paquete *paquete = crear_paquete(CONTEXTO);
    
    serializar_contexto(&EJECUTANDO->contexto, paquete);

    enviar_paquete(paquete, SOCKET_CPU);

    log_info(LOGGER_KERNEL,"Se envia el PID <%d> al CPU",EJECUTANDO->contexto.PID);
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
        log_info(LOGGER_KERNEL, "Se recibio un mensaje de desalojo");
        reemplazar_exec_por_nuevo();
        break;

    case PETICION:
        log_info(LOGGER_KERNEL, "Se recibio un mensaje de peticion");
        //I/O
        reemplazar_exec_por_nuevo();
        break;
    
    case BLOQUEAR:
        log_info(LOGGER_KERNEL, "Se recibio un mensaje de bloqueo");
        reemplazar_exec_por_nuevo();
        break;

    case TERMINAR:
        log_info(LOGGER_KERNEL, "Se recibio un mensaje de finalizacion");
        terminar_proceso(proceso);
        reemplazar_exec_por_nuevo();
        break;

    case WAIT:
        // cree una nueva struct dentro del contexto que tiene un char** parametros
        // me parecio lo mas generico para pasar todo el resto de instrucciones 
        // tipo I/O, F_OPEN, F_CLOSE, F_SEEK, F_READ, F_WRITE, F_TRUNCATE, WAIT, SIGNAL, CREATE_SEGMENT,

        // seria algo asi como
        // char* recurso = proceso->contexto->motivos_desalojo->parametros[0];

        // si recurso fuese un int habria que castearlo con la funcion atoi()
        // int cantidad = atoi(proceso->contexto->motivos_desalojo->parametros[1]);
        int recurso_id = get_id_recurso(proceso->contexto.motivos_desalojo->parametros[0]);
        if (INSTANCIAS_RECURSOS[recurso_id]==0){
            log_info(LOGGER_KERNEL, "Se recibio un mensaje de wait");
            reemplazar_exec_por_nuevo();

        }
        else{
            sem_wait(SEMAFOROS_RECURSOS[recurso_id]);
            log_info(LOGGER_KERNEL, "Se recibio un mensaje de wait del recurso %d", recurso_id);
            INSTANCIAS_RECURSOS[recurso_id]--;
        };
        break;

    case SIGNAL:
        // char* recurso = proceso->contexto->motivos_desalojo->parametros[0];
        // int cantidad = atoi(proceso->contexto->motivos_desalojo->parametros[1]);
        sem_signal(SEMAFOROS_RECURSOS[recurso_id]);
        log_info(LOGGER_KERNEL, "Se recibio un mensaje de signal del recurso %d", recurso_id);
        INSTANCIAS_RECURSOS[recurso_id]++;
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