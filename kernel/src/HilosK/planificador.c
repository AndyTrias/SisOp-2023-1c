#include "planificador.h"

void* planificador_corto(){
    log_info(LOGGER_KERNEL, "Inicia el planificador de corto plazo");
    sem_wait(&CORTO_PLAZO);
    log_info(LOGGER_KERNEL, "Empieza ciclo de instruccion");
    empezar_ciclo();
    while (1){
        recibir_de_cpu(SOCKET_CPU);
    }
    
}


void* planificador_largo(){
    log_info(LOGGER_KERNEL, "Inicia el planificador de largo plazo");
    while (1){
        sem_wait(&PROCESO_EN_NEW);
        sem_wait(&GRADO_MULTIPROGRAMACION);
        t_pcb* aux;
        aux= sacar_de_lista_new(0);

        agregar_a_lista_ready(aux);
       
        sem_post(&CORTO_PLAZO);

        log_info(LOGGER_KERNEL,"Se mando la señal al planificador de corto plazo");

    }
    
}