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

