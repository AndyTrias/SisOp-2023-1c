#include "globales.h"

t_log *LOGGER_KERNEL;
int SOCKET_CPU;
int SOCKET_MEMORIA;
int SOCKET_FILESYSTEM;

// Contador de procesos generados
int PID_COUNT = 0;

// Listas para los estados
t_list *LISTA_NEW;
t_list *LISTA_READY;
t_pcb *EJECUTANDO;
t_list *LISTAS_BLOCK;

// Variables de configuracion
char *ALGORITMO_PLANIFICACION;
int ESTIMACION_INICIAL;
float HRRN_ALFA;
int GRADO_MAX_MULTIPROGRAMACION;
char **RECURSOS;
char **INSTANCIAS_RECURSOS;

// Semaforos
sem_t PROCESO_EN_NEW;
sem_t PROCESO_EN_READY;
sem_t PROCESO_EN_BLOCK;
sem_t GRADO_MULTIPROGRAMACION;
sem_t CORTO_PLAZO;
pthread_mutex_t MUTEX_LISTA_NEW;
pthread_mutex_t MUTEX_LISTA_READY;
pthread_mutex_t MUTEX_LISTA_BLOCK;

// Temporales

t_temporal *TIEMPO_CORRIENDO;
t_temporal *TIEMPO_EN_CPU;

// Declaracion de monitores para las variables globales

// Para agregar a las listas

void agregar_a_lista_new(t_pcb *nuevo)
{
    pthread_mutex_lock(&MUTEX_LISTA_NEW);
    list_add(LISTA_NEW, nuevo);
    pthread_mutex_unlock(&MUTEX_LISTA_NEW);
    sem_post(&PROCESO_EN_NEW);
}

void agregar_a_lista_ready(t_pcb *nuevo)
{
    pthread_mutex_lock(&MUTEX_LISTA_READY);
    temporal_stop(nuevo->tiempo_desde_ult_ready);
    nuevo->tiempo_llegada_ready = temporal_gettime(nuevo->tiempo_desde_ult_ready) + temporal_gettime(TIEMPO_CORRIENDO);
    list_add(LISTA_READY, nuevo);
    log_info(LOGGER_KERNEL,  "Cola Ready <%s>: [%s]",ALGORITMO_PLANIFICACION, "ayuda loco");
    pthread_mutex_unlock(&MUTEX_LISTA_READY);
    sem_post(&PROCESO_EN_READY);

}

/*
void agregar_a_lista_block(t_pcb* nuevo){
    pthread_mutex_lock(&MUTEX_LISTA_BLOCK);
    list_add(LISTA_BLOCK, nuevo);
    pthread_mutex_unlock(&MUTEX_LISTA_BLOCK);
    sem_post(&PROCESO_EN_BLOCK);
}*/

// Para sacar elemento X de la lista
t_pcb *sacar_de_lista_new(int posicion)
{
    pthread_mutex_lock(&MUTEX_LISTA_NEW);
    t_pcb *pcb = list_remove(LISTA_NEW, posicion);
    pthread_mutex_unlock(&MUTEX_LISTA_NEW);
    return pcb;
}

t_pcb *sacar_de_lista_ready(int posicion)
{
    sem_wait(&PROCESO_EN_READY); // Me parece que es este semaforo el
    pthread_mutex_lock(&MUTEX_LISTA_READY);
    t_pcb *pcb = list_remove(LISTA_READY, posicion);
    temporal_resume(pcb->tiempo_desde_ult_ready);
    pthread_mutex_unlock(&MUTEX_LISTA_READY);
    return pcb;
}
void sacar_elemento_de_lista_ready(t_pcb *elemento)
{
    sem_wait(&PROCESO_EN_READY);
    pthread_mutex_lock(&MUTEX_LISTA_READY);
    temporal_resume(elemento->tiempo_desde_ult_ready);
    list_remove_element(LISTA_READY, elemento);
    pthread_mutex_unlock(&MUTEX_LISTA_READY);
}
/*
t_pcb* sacar_de_lista_block(int posicion){
    sem_wait(&PROCESO_EN_BLOCK);
    pthread_mutex_lock(&MUTEX_LISTA_BLOCK);
    t_pcb* pcb = list_remove(LISTA_BLOCK, posicion);
    pthread_mutex_unlock(&MUTEX_LISTA_BLOCK);
    return pcb;
}*/

// Monitores para los get
t_pcb *get_de_lista_ready(int posicion)
{
    sem_wait(&PROCESO_EN_READY);
    pthread_mutex_lock(&MUTEX_LISTA_READY);
    t_pcb *pcb = list_get(LISTA_READY, posicion);
    pthread_mutex_unlock(&MUTEX_LISTA_READY);
    sem_post(&PROCESO_EN_READY);

    return pcb;
}

//cambios de estado
void cambio_de_estado(int pid, char*anterior, char* nuevo){
    log_info(LOGGER_KERNEL,"PID: <%d> - Estado Anterior: <%s> - Estado Actual: <%s>",pid,anterior,nuevo);
}
