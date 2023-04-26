#include "globales.h"

t_log* logger_kernel;

//Contador de procesos generados
int PID_count = 0;

//Listas para los estados
t_list* lista_new;
t_list* lista_ready;
t_list* lista_exec;
t_list* lista_block;

