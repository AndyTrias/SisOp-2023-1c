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
    
        if ( 1 /*no hay recursos disponibles*/){
            log_info(LOGGER_KERNEL, "Se recibio un mensaje de wait");
            reemplazar_exec_por_nuevo();

        }
        else{
            //se le da el recurso;
        };
        break;
    
    case IO:
        log_info(LOGGER_KERNEL, "Se recibio un mensaje de petición I/O");
        t_instruccion* instruccion_utilizable = list_get(proceso->contexto.instrucciones, proceso->contexto.program_counter - 1);
        int tiempo_bloqueo = atoi(instruccion_utilizable->parametros[0]);
        agregar_a_lista_block(proceso);
        // La pregunta más grande es, cómo hacer para que se envíen instrucciones al CPU mientras que Kernerl ejecuta el I/O. El CPU debería recibir algo no?
        // Creo tener una solución para lo de abajo pero no para lo de arriba si de verdad es un problema. En un toque lo sigo. 
        reemplazar_exec_por_nuevo(); //¡¡¡¡ Funciona!!!! Pero el tema es que en este caso se queda sin procesos en ready y lo que hace es esperar a que haya un proceso en ready 
                                    // Eso está bien pero mientras debería poder ejecutar esta instrucción y al mismo tiempo debería revisar si hay un nuevo proceso en ready
                                    // imagino que eso se hace con hilos pero bueno.
        log_info(LOGGER_KERNEL, "PID: %d - Ejecuta IO: %d", proceso->contexto.PID, tiempo_bloqueo);
        sleep(tiempo_bloqueo);
        agregar_a_lista_ready(sacar_de_lista_block(0)); // Esta parte parece que funciona porque funcionó con el Proceso 0 y el 1 
                                                        // Imagino que podría verse afectado cuando haya una instrucción I/O que termine antes que el primero que estaba en la cola. O acaso no comparten la misma lista de Block 
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