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

void* comunicacion_fs(){

   while(1){
    int cod_op = recibir_operacion(SOCKET_FILESYSTEM);
    
    int size;
    void* buffer = recibir_buffer(&size, SOCKET_FILESYSTEM);
    int* desplazamiento = malloc(sizeof(int));
    *desplazamiento = 0;

    char* nombre_archivo;// = deserealizar_nombre(buffer, desplazamiento);

    switch (cod_op){
    case EXISTE:
        pthread_mutex_unlock(&BLOQUEADOS_FS);
        sem_post(&RESPUESTA_FS);
        break;
    case NO_EXISTE:
        pthread_mutex_unlock(&BLOQUEADOS_FS);
        solicitar_creacion();
        break;
    case OP_TERMINADA: //esto es cuando termina el f truncate read y write, 
    //necesito el nombre del archivo que termino de hacer eso para desbloquear 
    //al proceso bloquedo por el archivo
        pthread_mutex_unlock(&BLOQUEADOS_FS);
        char* nombre_archivo;// = deserealizar_nombre(buffer, desplazamiento);
        desbloquear_de_fs(nombre_archivo);
        break;
    default:
        break;
    }
   }
   
}