#include "mensajes.h"

t_paquete *crear_paquete(int cod_op)
{
	t_paquete *paquete = malloc(sizeof(t_paquete));
	paquete->codigo_operacion = cod_op;
	crear_buffer(paquete);
	return paquete;
}

void crear_buffer(t_paquete *paquete)
{
	paquete->buffer = malloc(sizeof(t_buffer));
	paquete->buffer->size = 0;
	paquete->buffer->stream = NULL;
}

void *serializar_paquete(t_paquete *paquete, int bytes)
{
	void *magic = malloc(bytes);
	int desplazamiento = 0;

	memcpy(magic + desplazamiento, &(paquete->codigo_operacion), sizeof(int));
	desplazamiento += sizeof(int);
	memcpy(magic + desplazamiento, &(paquete->buffer->size), sizeof(int));
	desplazamiento += sizeof(int);
	memcpy(magic + desplazamiento, paquete->buffer->stream, paquete->buffer->size);
	desplazamiento += paquete->buffer->size;

	return magic;
}

void eliminar_paquete(t_paquete *paquete)
{
	free(paquete->buffer->stream);
	free(paquete->buffer);
	free(paquete);
}

void enviar_mensaje(char *mensaje, int socket_cliente)
{
	t_paquete *paquete = malloc(sizeof(t_paquete));

	paquete->codigo_operacion = MENSAJE;
	paquete->buffer = malloc(sizeof(t_buffer));
	paquete->buffer->size = strlen(mensaje) + 1;
	paquete->buffer->stream = malloc(paquete->buffer->size);
	memcpy(paquete->buffer->stream, mensaje, paquete->buffer->size);

	int bytes = paquete->buffer->size + 2 * sizeof(int);

	void *a_enviar = serializar_paquete(paquete, bytes);

	send(socket_cliente, a_enviar, bytes, 0);

	free(a_enviar);
	eliminar_paquete(paquete);
}

void agregar_a_paquete_dato_serializado(t_paquete *paquete, void *valor, int tamanio)
{
	paquete->buffer->stream = realloc(paquete->buffer->stream, paquete->buffer->size + tamanio);

	// Agrega el valor
	memcpy(paquete->buffer->stream + paquete->buffer->size, valor, tamanio);

	paquete->buffer->size += tamanio;
}

void agregar_a_paquete(t_paquete *paquete, void *valor, int bytes)
{
	t_buffer *buffer = paquete->buffer;

	buffer->stream = realloc(buffer->stream, buffer->size + bytes);

	memcpy(buffer->stream + buffer->size, valor, bytes);

	buffer->size += bytes;
}

void enviar_paquete(t_paquete *paquete, int socket_cliente)
{
	int bytes = paquete->buffer->size + 2 * sizeof(int);
	void *a_enviar = serializar_paquete(paquete, bytes);

	send(socket_cliente, a_enviar, bytes, 0);

	free(a_enviar);
}

int recibir_operacion(int socket_cliente)
{
	int cod_op;
	if (recv(socket_cliente, &cod_op, sizeof(int), MSG_WAITALL) > 0)
		return cod_op;
	else
	{
		close(socket_cliente);
		return -1;
	}
}

void *recibir_buffer(int *size, int socket_cliente)
{
	void *buffer;

	recv(socket_cliente, size, sizeof(int), MSG_WAITALL);
	buffer = malloc(*size);
	recv(socket_cliente, buffer, *size, MSG_WAITALL);

	return buffer;
}

void recibir_mensaje(int socket_cliente)
{
	int size;
	char *buffer = recibir_buffer(&size, socket_cliente);
	printf("%s \n", buffer);
	free(buffer);
}


void obtener_identificador(char *identificador, int socket_cliente)
{
	int size;
	char *buffer = recibir_buffer(&size, socket_cliente);
	strcpy(identificador, buffer);
	free(buffer);
}

void serializar_contexto(t_ctx *ctx, t_paquete *paquete)
{
	// Serialio PID y PC
	agregar_a_paquete_dato_serializado(paquete, &ctx->PID, sizeof(ctx->PID));
	agregar_a_paquete_dato_serializado(paquete, &ctx->program_counter, sizeof(ctx->program_counter));
	agregar_a_paquete_dato_serializado(paquete, &ctx->cant_instrucciones, sizeof(ctx->cant_instrucciones));

	
	// Serializo Instrucciones
	serializar_instrucciones(ctx->instrucciones, paquete);

	// Serializo Registros
	serializar_registros(&ctx->registros, paquete);
	
	// serializo moivos de desalojo
	int tamanio_parametro;
	agregar_a_paquete_dato_serializado(paquete, &(ctx->motivos_desalojo->cantidad_parametros), sizeof(int));

	for (int i = 0; i < ctx->motivos_desalojo->cantidad_parametros; i++)
	{
		tamanio_parametro = strlen(ctx->motivos_desalojo->parametros[i]) + 1;
		agregar_a_paquete_dato_serializado(paquete, &tamanio_parametro, sizeof(int));
		agregar_a_paquete_dato_serializado(paquete, ctx->motivos_desalojo->parametros[i], tamanio_parametro);		
	}

	
}

void serializar_instrucciones(t_list *instrucciones, t_paquete *paquete)
{
	int cant_instrucciones = list_size(instrucciones);
	for (int i = 0; i < cant_instrucciones; i++)
	{
		t_instruccion *instruccion = list_get(instrucciones, i);
		serializar_instruccion(instruccion, paquete);
	}
}
void serializar_instruccion(t_instruccion *instruccion, t_paquete *paquete)
{

	int tamanio_parametro;

	agregar_a_paquete_dato_serializado(paquete, &(instruccion->operacion), sizeof(t_operacion));

	agregar_a_paquete_dato_serializado(paquete, &(instruccion->cantidad_parametros), sizeof(int));

	for (int i = 0; i < instruccion->cantidad_parametros; i++)
	{

		// Agrego el tamanio del parametro porque es un char*
		tamanio_parametro = strlen(instruccion->parametros[i]) + 1;
		agregar_a_paquete_dato_serializado(paquete, &tamanio_parametro, sizeof(int));

		// Agrego el parametro
		agregar_a_paquete_dato_serializado(paquete, instruccion->parametros[i], tamanio_parametro);
	}
}
void serializar_registros(t_registros *registros, t_paquete *paquete)
{
	agregar_a_paquete_dato_serializado(paquete, &(registros->AX), sizeof(registros->AX));
	agregar_a_paquete_dato_serializado(paquete, &(registros->BX), sizeof(registros->BX));
	agregar_a_paquete_dato_serializado(paquete, &(registros->CX), sizeof(registros->CX));
	agregar_a_paquete_dato_serializado(paquete, &(registros->DX), sizeof(registros->DX));
	agregar_a_paquete_dato_serializado(paquete, &(registros->EAX), sizeof(registros->EAX));
	agregar_a_paquete_dato_serializado(paquete, &(registros->EBX), sizeof(registros->EBX));
	agregar_a_paquete_dato_serializado(paquete, &(registros->ECX), sizeof(registros->ECX));
	agregar_a_paquete_dato_serializado(paquete, &(registros->EDX), sizeof(registros->EDX));
	agregar_a_paquete_dato_serializado(paquete, &(registros->RAX), sizeof(registros->RAX));
	agregar_a_paquete_dato_serializado(paquete, &(registros->RBX), sizeof(registros->RBX));
	agregar_a_paquete_dato_serializado(paquete, &(registros->RCX), sizeof(registros->RCX));
	agregar_a_paquete_dato_serializado(paquete, &(registros->RDX), sizeof(registros->RDX));
}

t_ctx *deserializar_contexto(void *buffer)
{
	t_ctx *ctx = malloc(sizeof(t_ctx));
	int desplazamiento = 0;


	// Deserializo PID, PC y cant_instrucciones
	memcpy(&ctx->PID, buffer + desplazamiento, sizeof(ctx->PID));
	desplazamiento += sizeof(ctx->PID);
	memcpy(&ctx->program_counter, buffer + desplazamiento, sizeof(ctx->program_counter));
	desplazamiento += sizeof(ctx->program_counter);
	memcpy(&ctx->cant_instrucciones, buffer + desplazamiento, sizeof(ctx->cant_instrucciones));
	desplazamiento += sizeof(ctx->cant_instrucciones);

	// Deserializo Instrucciones
	ctx->instrucciones = list_create();

	for (int i = 0; i < ctx->cant_instrucciones; i++)
	{
		list_add(ctx->instrucciones, deserealizar_instruccion(buffer, &desplazamiento));

	}

	// Deserializo Registros
	ctx->registros = deserealizar_registros(buffer, &desplazamiento); 


	// deserializo motivos de desalojo
	ctx->motivos_desalojo = malloc(sizeof(t_parametros_variables));
	memcpy(&ctx->motivos_desalojo->cantidad_parametros, buffer + desplazamiento, sizeof(int));
	desplazamiento += sizeof(int);

	ctx->motivos_desalojo->parametros = malloc(sizeof(char *) * ctx->motivos_desalojo->cantidad_parametros);
	for (int i = 0; i < ctx->motivos_desalojo->cantidad_parametros; i++)
	{
		int tamanio_parametro;
		memcpy(&tamanio_parametro, buffer + desplazamiento, sizeof(int));
		desplazamiento += sizeof(int);

		ctx->motivos_desalojo->parametros[i] = malloc(tamanio_parametro);
		memcpy(ctx->motivos_desalojo->parametros[i], buffer + desplazamiento, tamanio_parametro);
		desplazamiento += tamanio_parametro;
	}

	free(buffer);
	
	return ctx;

}

t_instruccion *deserealizar_instruccion(void *buffer, int *desplazamiento)
{
    t_instruccion *instruccion_deserializada = malloc(sizeof(t_instruccion));
    int tamanio_parametro;

    memcpy(&instruccion_deserializada->operacion, buffer + *desplazamiento, sizeof(t_operacion));
    *desplazamiento += sizeof(t_operacion);

    memcpy(&instruccion_deserializada->cantidad_parametros, buffer + *desplazamiento, sizeof(int));
    *desplazamiento += sizeof(int);

    instruccion_deserializada->parametros = malloc(sizeof(char *) * instruccion_deserializada->cantidad_parametros);
    for (int i = 0; i < instruccion_deserializada->cantidad_parametros; i++)
    {
        memcpy(&tamanio_parametro, buffer + *desplazamiento, sizeof(int));
        *desplazamiento += sizeof(int);

        instruccion_deserializada->parametros[i] = malloc(tamanio_parametro);
        memcpy(instruccion_deserializada->parametros[i], buffer + *desplazamiento, tamanio_parametro);
        *desplazamiento += tamanio_parametro;
    }
    
    return instruccion_deserializada;
}

t_registros deserealizar_registros(void *buffer, int*desplazamiento)
{
    t_registros registros;

    memcpy(registros.AX, buffer + *desplazamiento, sizeof(registros.AX));
    *desplazamiento += sizeof(registros.AX);

    memcpy(registros.BX, buffer + *desplazamiento, sizeof(registros.BX));
    *desplazamiento += sizeof(registros.BX);

    memcpy(registros.CX, buffer + *desplazamiento, sizeof(registros.CX));
    *desplazamiento += sizeof(registros.CX);

    memcpy(registros.DX, buffer + *desplazamiento, sizeof(registros.DX));
    *desplazamiento += sizeof(registros.DX);

    memcpy(registros.EAX, buffer + *desplazamiento, sizeof(registros.EAX));
    *desplazamiento += sizeof(registros.EAX);

    memcpy(registros.EBX, buffer + *desplazamiento, sizeof(registros.EBX));
    *desplazamiento += sizeof(registros.EBX);

    memcpy(registros.ECX, buffer + *desplazamiento, sizeof(registros.ECX));
    *desplazamiento += sizeof(registros.ECX);

    memcpy(registros.EDX, buffer + *desplazamiento, sizeof(registros.EDX));
    *desplazamiento += sizeof(registros.EDX);

    memcpy(registros.RAX, buffer + *desplazamiento, sizeof(registros.RAX));
    *desplazamiento += sizeof(registros.RAX);

    memcpy(registros.RBX, buffer + *desplazamiento, sizeof(registros.RBX));
    *desplazamiento += sizeof(registros.RBX);

    memcpy(registros.RCX, buffer + *desplazamiento, sizeof(registros.RCX));
    *desplazamiento += sizeof(registros.RCX);

    memcpy(registros.RDX, buffer + *desplazamiento, sizeof(registros.RDX));
    *desplazamiento += sizeof(registros.RDX);

    return registros;
}


