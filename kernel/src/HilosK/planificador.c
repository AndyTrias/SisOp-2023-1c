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

        admitir_proceso();
       
        sem_post(&CORTO_PLAZO);

        log_info(LOGGER_KERNEL,"Se mando la señal al planificador de corto plazo");

    }
    
}
void* recibir_consolas(int socket_servidor){
    
    while(1){
        conectar_consola(socket_servidor);
    }
}