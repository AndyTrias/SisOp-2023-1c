#include "globales.h"

t_log* LOGGER_KERNEL;

//Contador de procesos generados
int PID_COUNT = 0;

//Listas para los estados
t_list* LISTA_NEW;
t_list* LISTA_READY;
t_list* LISTA_EXEC;
t_list* LISTA_BLOCK;

// Variables de configuracion
char* ALGORITMO_PLANIFICACION;
int ESTIMACION_INICIAL;
float HRRN_ALFA;
int GRADO_MAX_MULTIPROGRAMACION;
char** RECURSOS;
char** INSTANCIAS_RECURSOS;

//Semaforos
sem_t CONSOLA_CONECTADA;
pthread_mutex_t MUTEX_LISTA_NEW;
pthread_mutex_t MUTEX_LISTA_READY;
pthread_mutex_t MUTEX_LISTA_EXEC;
pthread_mutex_t MUTEX_LISTA_BLOCK;

int SOCKET_CPU;

//Declaracion de monitores para las variables globales

//Para agregar a las listas

void agregar_a_lista_new(t_pcb* nuevo){
    pthread_mutex_lock(&MUTEX_LISTA_NEW);
    list_add(LISTA_NEW, nuevo);
    pthread_mutex_unlock(&MUTEX_LISTA_NEW);
}

void agregar_a_lista_ready(t_pcb *nuevo){
    pthread_mutex_lock(&MUTEX_LISTA_READY);
    list_add(LISTA_READY, nuevo);
    pthread_mutex_unlock(&MUTEX_LISTA_READY);
    
    log_info(LOGGER_KERNEL, "PID <%d> a ready en base al algortmo de planificacion <%s>",nuevo->contexto.PID, ALGORITMO_PLANIFICACION);
}

void agregar_a_lista_exec(t_pcb* nuevo){
    pthread_mutex_lock(&MUTEX_LISTA_EXEC);
    list_add(LISTA_EXEC, nuevo);
    pthread_mutex_unlock(&MUTEX_LISTA_EXEC);
}

void agregar_a_lista_block(t_pcb* nuevo){
    pthread_mutex_lock(&MUTEX_LISTA_BLOCK);
    list_add(LISTA_BLOCK, nuevo);
    pthread_mutex_unlock(&MUTEX_LISTA_BLOCK);
}

//Para sacar elemento X de la lista 
t_pcb* sacar_de_lista_new(int posicion){
    pthread_mutex_lock(&MUTEX_LISTA_NEW);
    t_pcb* pcb =list_remove(LISTA_NEW, posicion);
    pthread_mutex_unlock(&MUTEX_LISTA_NEW);
    return pcb;
}

t_pcb* sacar_de_lista_ready(int posicion){
    pthread_mutex_lock(&MUTEX_LISTA_READY);
    t_pcb * pcb = list_remove(LISTA_READY, posicion);
    pthread_mutex_unlock(&MUTEX_LISTA_READY);

    return pcb;
}

t_pcb* sacar_de_lista_exec(int posicion){
    pthread_mutex_lock(&MUTEX_LISTA_EXEC);
    t_pcb* pcb = list_remove(LISTA_EXEC, posicion);
    pthread_mutex_unlock(&MUTEX_LISTA_EXEC);
    return pcb;
}

t_pcb* sacar_de_lista_block(int posicion){
    pthread_mutex_lock(&MUTEX_LISTA_BLOCK);
    t_pcb* pcb = list_remove(LISTA_BLOCK, posicion);
    pthread_mutex_unlock(&MUTEX_LISTA_BLOCK);
    return pcb;
}