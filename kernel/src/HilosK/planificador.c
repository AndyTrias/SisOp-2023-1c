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

// no sé que es
/*
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
        sem_post(&RESPUESTA_FS);
        break;
    case NO_EXISTE:
        solicitar_creacion(nombre_archivo);
        break;
    case OP_TERMINADA:
        desbloquear_de_fs(nombre_archivo);
        break;
    default:
        break;
    }
   }
   
}
*/