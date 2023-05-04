#include "gestion_de_procesos.h"

void planificador(){
    sem_wait(&CONSOLA_CONECTADA);// espera a que se conecte una consola antes de que comience
    while (1){
        //sem_wait(&ALGO_EN_NEW)
        //admitir_proceso()
    }
    
}