#ifndef MENSAJES_H_
#define MENSAJES_H_

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include <commons/log.h>
#include <commons/collections/list.h>
#include "planificacion/pcb.h"


// make necessary includes
typedef enum
{
    INSTRUCCIONES, // Se utiliza de consola a kernel
    MENSAJE, // Se utiliza de kernel a consola
    CONTEXTO, // Se utiliza de kernel a cpu
    DESALOJAR,
    BLOQUEAR,
    TERMINAR
} op_code;

typedef enum
{
    F_READ,
    F_WRITE, 
    MOV_IN,
    MOV_OUT,
    F_TRUNCATE,
    F_SEEK,
    CREATE_SEGMENT, 
    IO,
    SET,
    WAIT,
    SIGNAL,
    F_OPEN,
    F_CLOSE,
    DELETE_SEGMENT, 
    EXIT, // Ver como la definimos
    YIELD 
} t_operacion;

typedef struct
{
    t_operacion operacion;
    int cantidad_parametros;
    char **parametros;
} t_instruccion;


// Otra opcipn
// typedef struct
// {
//     t_operacion operacion;
//     char *parametro1;
//     char *parametro2;
//     char *parametro3;

// } t_instruccion;

typedef struct
{
    int size;
    void *stream;
} t_buffer;

typedef struct
{
    op_code codigo_operacion;
    t_buffer *buffer;
} t_paquete;

// Funciones del tp0
t_paquete *crear_paquete(int);
void crear_buffer(t_paquete *);
void *serializar_paquete(t_paquete *, int);
void enviar_mensaje(char *, int);
int recibir_operacion(int);
void recibir_mensaje(int);
void agregar_a_paquete_dato_serializado(t_paquete *, void *, int);
void enviar_paquete(t_paquete *, int);
void *recibir_paquete(int, int*);
void* recibir_buffer(int* , int);


// Serializaciones + Deserealizaciones que usan las funciones de arriba
void serializar_contexto(t_ctx*, t_paquete*);
void serializar_instrucciones(t_list *, t_paquete* );
void serializar_instruccion(t_instruccion *, t_paquete *);
void serializar_registros(t_registros *, t_paquete *);
t_ctx *deserializar_contexto(void *);
t_registros deserealizar_registros(void *, int*);
t_instruccion* deserealizar_instruccion(void*, int* );







#endif
