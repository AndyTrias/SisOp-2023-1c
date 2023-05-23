#include "planificador_corto_plazo.h"

void* planificador_corto(){
    log_info(LOGGER_KERNEL, "Inicia el planificador de corto plazo");
    sem_wait(&CORTO_PLAZO);
    log_info(LOGGER_KERNEL, "Empieza ciclo de instruccion");
    empezar_ciclo_si_vacio();
    while (1){
        //recibir_de_cpu(SOCKET_CPU);
    }
    
}