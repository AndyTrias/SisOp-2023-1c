#ifndef PCB_H_
#define PCB_H_

#include <commons/collections/list.h>
#include <commons/temporal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct
{
    char AX[4];
    char BX[4];
    char CX[4];
    char DX[4];

    char EAX[8];
    char EBX[8];
    char ECX[8];
    char EDX[8];

    char RAX[16];
    char RBX[16];
    char RCX[16];
    char RDX[16];
} t_registros;

typedef struct
{
    int cantidad_parametros;
    char **parametros;
} t_parametros_variables;

typedef struct
{
    int PID;
    int cant_instrucciones;
    t_list *instrucciones;
    int program_counter;
    t_registros registros;
    t_parametros_variables *motivos_desalojo;
    t_list* tabla_segmentos;
} t_ctx;

typedef struct
{
    t_ctx contexto;
    float estimado_prox_rafaga;
    t_temporal *tiempo_desde_ult_ready; // cuando pasa a ready usar temporal_stop
    int64_t tiempo_llegada_ready;       // cuando pasa a ready usar  = temporal_gettime(tiempo_desde_ult_ready)
    t_list *archivos_abiertos;
    int socket_consola;
} t_pcb;

void agregar_parametro_desalojo(t_ctx* ctx, char* parametro);
void agregar_parametro_variable(t_parametros_variables* , char* );

char get_pid(t_pcb*);
char* mostrar_pids(t_list* lista);
void concatenarCharACadena(char c, char *cadena);

void liberar_parametros_variables(t_parametros_variables* );
void liberar_parametros_desalojo(t_ctx* ctx);




#endif
