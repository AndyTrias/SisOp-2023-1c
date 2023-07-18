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

typedef enum
{
    F_READ,
    F_WRITE, 
    MOV_OUT,
    F_TRUNCATE,
    F_SEEK,
    MOV_IN,
    CREATE_SEGMENT, 
    IO,
    SET,
    WAIT,
    SIGNAL,
    F_OPEN,
    F_CLOSE,
    DELETE_SEGMENT, 
    EXIT, 
    YIELD,
    SEG_FAULT
} t_operacion;


typedef struct
{
    t_operacion operacion;
    int cantidad_parametros;
    char **parametros;
} t_instruccion;

typedef struct {
    int id_segmento;
    void* base;
    int tamanio;
} t_segmento;


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
    int64_t tiempo_llegada_ready;       // cuando pasa a ready usar  = temporal_gettime(tiempo_desde_ult_ready)
    t_list *archivos_abiertos;
    t_list *recursos_en_uso;
    int socket_consola;
} t_pcb;

void agregar_parametro_desalojo(t_ctx* ctx, char* parametro);
void agregar_parametro_variable(t_parametros_variables* , char* );

char get_pid(t_pcb*);
char* mostrar_pids(t_list* lista);
void concatenarCharACadena(char c, char *cadena);

void liberar_contexto(t_ctx *ctx);
void liberar_instruccion(t_instruccion *instruccion);
void liberar_segmento(t_segmento* segmento);
void liberar_parametros_variables(t_parametros_variables* );
void liberar_parametros_desalojo(t_ctx* ctx);





#endif
