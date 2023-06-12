#ifndef GLOBALES_H_
#define GLOBALES_H_

#include <utils/logs.h>
#include <commons/collections/list.h>
#include <commons/string.h>
#include <utils/planificacion/pcb.h>
#include <pthread.h>
#include <semaphore.h>
#include <string.h>

extern t_log* LOGGER_KERNEL;
extern int SOCKET_CPU;
extern int SOCKET_MEMORIA;
extern int SOCKET_FILESYSTEM;

//Contador de procesos generados
extern int PID_COUNT;

//Tabla global de archivos abiertos
extern t_list* TABLA_GLOBAL_DE_ARCHIVOS_ABIERTOS 

//Listas para los estados
extern t_list* LISTA_NEW;
extern t_list* LISTA_READY;
extern t_pcb* EJECUTANDO;
extern t_list* LISTAS_BLOCK;


// Variables de configuracion
extern char* ALGORITMO_PLANIFICACION;
extern int ESTIMACION_INICIAL;
extern float HRRN_ALFA;
extern int GRADO_MAX_MULTIPROGRAMACION;
extern char** RECURSOS;
extern char** INSTANCIAS_RECURSOS;

//Semaforos
extern sem_t PROCESO_EN_NEW;
extern sem_t PROCESO_EN_READY;
extern sem_t GRADO_MULTIPROGRAMACION;
extern sem_t CORTO_PLAZO;
extern sem_t ARCHIVO_ABIERTO;
extern pthread_mutex_t MUTEX_LISTA_NEW;
extern pthread_mutex_t MUTEX_LISTA_READY;
extern pthread_mutex_t MUTEX_TABLA_ARCHIVOS;



extern t_temporal* TIEMPO_CORRIENDO;
extern t_temporal* TIEMPO_EN_CPU;

//Declaro las funciones de los monitores
void agregar_a_lista_new(t_pcb* );
void agregar_a_lista_ready(t_pcb* );
void agregar_a_lista_exec(t_pcb* );


t_pcb* sacar_de_lista_new(int);
t_pcb* sacar_de_lista_ready(int);
void sacar_elemento_de_lista_ready(t_pcb* );
t_pcb* sacar_de_lista_exec(int);


t_pcb* get_de_lista_ready(int);

//cambios de estado
void cambio_de_estado();

#endif