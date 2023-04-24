#include "crear_instrucciones.h"

t_operacion obtener_codigo_instruccion(char *identificador)
{
    t_operacion op;
    if (!strcmp("F_READ", identificador))
        op = F_READ;

    if (!strcmp("F_WRITE", identificador))
        op = F_WRITE;

    if (!strcmp("SET", identificador))
        op = SET;

    if (!strcmp("MOV_IN", identificador))
        op = MOV_IN;

    if (!strcmp("MOV_OUT", identificador))
        op = MOV_OUT;

    if (!strcmp("F_TRUNCATE", identificador))
        op = F_TRUNCATE;

    if (!strcmp("F_SEEK", identificador))
        op = F_SEEK;

    if (!strcmp("CREATE_SEGMENT", identificador))
        op = CREATE_SEGMENT;

    if (!strcmp("IO", identificador))
        op = IO;

    if (!strcmp("WAIT", identificador))
        op = WAIT;

    if (!strcmp("SIGNAL", identificador))
        op = SIGNAL;

    if (!strcmp("F_OPEN", identificador))
        op = F_OPEN;

    if (!strcmp("F_CLOSE", identificador))
        op = F_CLOSE;

    if (!strcmp("DELETE_SEGMENT", identificador))
        op = DELETE_SEGMENT;

    if (!strcmp("EXIT", identificador))
        op = EXIT;

    if (!strcmp("YIELD", identificador))
        op = YIELD;

    return op;
}

t_instruccion* crear_estructura_instruccion(char* buffer) {
    char** linea_de_instruccion = string_split(buffer, " ");
    
    t_instruccion *instruccion = malloc(sizeof(t_instruccion));
    instruccion->parametros = malloc(sizeof(char **));
    
    instruccion->cantidad_parametros = 0;

    instruccion->operacion = obtener_codigo_instruccion(linea_de_instruccion[0]);
    for (int i = 1; linea_de_instruccion[i] != NULL; i++)
    {
      instruccion->cantidad_parametros++;
      instruccion->parametros[i - 1] = string_duplicate(linea_de_instruccion[i]);
      
    }

    return instruccion;
}

int serializar_instruccion(t_instruccion *instruccion, char *buffer) {
    int desplazamiento = 0;
    int size_to_send;

    memcpy(buffer + desplazamiento, &(instruccion->operacion), sizeof(t_operacion));
    desplazamiento += sizeof(t_operacion);

    memcpy(buffer + desplazamiento, &(instruccion->cantidad_parametros), sizeof(int));
    desplazamiento += sizeof(int);

    for (int i = 0; i < instruccion->cantidad_parametros; i++) {
        size_to_send = strlen(instruccion->parametros[i]) + 1;
        memcpy(buffer + desplazamiento, &(size_to_send), sizeof(int));
        desplazamiento += sizeof(int);

        memcpy(buffer + desplazamiento, instruccion->parametros[i], size_to_send);
        desplazamiento += size_to_send;
    }

    return desplazamiento;
}

void agregar_a_paquete(t_paquete* paquete, void* valor, int tamanio)
{
    paquete->buffer->stream = realloc(paquete->buffer->stream, paquete->buffer->size + tamanio + sizeof(int));

	// Agrega el tamanio
    memcpy(paquete->buffer->stream + paquete->buffer->size, &tamanio, sizeof(int));
	
    // Agrega el valor
    memcpy(paquete->buffer->stream + paquete->buffer->size + sizeof(int), valor, tamanio);

	paquete->buffer->size += tamanio + sizeof(int);
}