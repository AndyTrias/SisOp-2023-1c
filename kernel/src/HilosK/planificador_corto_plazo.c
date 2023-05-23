#include "planificador_corto_plazo.h"

void planificador_corto(){
    sem_wait(&CORTO_PLAZO);
    while (1){
        empezar_ciclo_si_vacio();
        recibir_de_cpu(SOCKET_CPU);
    }
    
}