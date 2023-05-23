#include "planificador_largo_plazo.h"

void planificador_largo(){
    
    sem_wait(&CONSOLA_CONECTADA);// espera a que se conecte una consola antes de que comience
    while (1){
        sem_wait(&GRADO_MULTIPROGRAMACION); //INICIALIZAR EL SEM EN N = GRADO DE MULTIPROGRAMACION
        t_pcb* aux;
        aux= sacar_de_lista_new(0);

        agregar_a_lista_ready(aux);

        log_info(LOGGER_KERNEL, "se agrego un proceso a ready");//lo puse para ver si se ejecutaba
       
        sem_post(&CORTO_PLAZO);

    }
    
}