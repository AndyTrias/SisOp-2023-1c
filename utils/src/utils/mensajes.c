#include "mensajes.h"


t_paquete* crear_paquete(int cod_op){
	t_paquete* paquete = malloc(sizeof(t_paquete));
	paquete->codigo_operacion = cod_op;
	crear_buffer(paquete);
	return paquete;
}

void crear_buffer(t_paquete* paquete){
	paquete->buffer = malloc(sizeof(t_buffer));
	paquete->buffer->size = 0;
	paquete->buffer->stream = NULL;
}

void* serializar_paquete(t_paquete* paquete, int bytes){
	void * magic = malloc(bytes);
	int desplazamiento = 0;

	memcpy(magic + desplazamiento, &(paquete->codigo_operacion), sizeof(int));
	desplazamiento+= sizeof(int);
	memcpy(magic + desplazamiento, &(paquete->buffer->size), sizeof(int));
	desplazamiento+= sizeof(int);
	memcpy(magic + desplazamiento, paquete->buffer->stream, paquete->buffer->size);
	desplazamiento+= paquete->buffer->size;

	return magic;
}

void eliminar_paquete(t_paquete* paquete){
	free(paquete->buffer->stream);
	free(paquete->buffer);
	free(paquete);
}

void enviar_mensaje(char* mensaje, int socket_cliente){
	t_paquete* paquete = malloc(sizeof(t_paquete));

	paquete->codigo_operacion = MENSAJE;
	paquete->buffer = malloc(sizeof(t_buffer));
	paquete->buffer->size = strlen(mensaje) + 1;
	paquete->buffer->stream = malloc(paquete->buffer->size);
	memcpy(paquete->buffer->stream, mensaje, paquete->buffer->size);

	int bytes = paquete->buffer->size + 2*sizeof(int);

	void* a_enviar = serializar_paquete(paquete, bytes);

	send(socket_cliente, a_enviar, bytes, 0);

	free(a_enviar);
	eliminar_paquete(paquete);
}

void agregar_a_paquete_dato_serializado(t_paquete* paquete, void* valor, int tamanio)
{
    paquete->buffer->stream = realloc(paquete->buffer->stream, paquete->buffer->size + tamanio);

    // Agrega el valor
    memcpy(paquete->buffer->stream + paquete->buffer->size, valor, tamanio);

	paquete->buffer->size += tamanio;
}

void agregar_a_paquete(t_paquete *paquete, void *valor, int bytes) {
	t_buffer *buffer = paquete->buffer;
	
	buffer->stream = realloc(buffer->stream, buffer->size + bytes);
	
	memcpy(buffer->stream + buffer->size, valor, bytes);
	
	buffer->size += bytes;
}


void enviar_paquete(t_paquete* paquete, int socket_cliente) {
	int bytes = paquete->buffer->size + 2*sizeof(int);
	void* a_enviar = serializar_paquete(paquete, bytes);

	send(socket_cliente, a_enviar, bytes, 0);

	free(a_enviar);
}

int recibir_operacion(int socket_cliente) {
	int cod_op;
	if(recv(socket_cliente, &cod_op, sizeof(int), MSG_WAITALL) > 0)
		return cod_op;
	else
	{
		close(socket_cliente);
		return -1;
	}
}

void* recibir_buffer(int* size, int socket_cliente) {
	void * buffer;

	recv(socket_cliente, size, sizeof(int), MSG_WAITALL);
	buffer = malloc(*size);
	recv(socket_cliente, buffer, *size, MSG_WAITALL);

	return buffer;
}

void recibir_mensaje(int socket_cliente) {
	int size;
	char* buffer = recibir_buffer(&size, socket_cliente);
	printf("%s \n", buffer);
	free(buffer);
}

void* recibir_paquete(int socket_cliente) {
	int size;
	void * buffer;
	buffer = recibir_buffer(&size, socket_cliente);

	return buffer;
}

void obtener_identificador(char* identificador, int socket_cliente) {
	int size;
	char* buffer = recibir_buffer(&size, socket_cliente);
	strcpy(identificador, buffer);
	free(buffer);
}

void serializar_contexto(t_ctx *ctx, t_paquete *paquete){
    // Serialio PID y PC
    agregar_a_paquete_dato_serializado(paquete, &ctx->PID, sizeof(ctx->PID));
    agregar_a_paquete_dato_serializado(paquete, &ctx->program_counter, sizeof(ctx->program_counter));

    // Serializo Instrucciones
    serializar_instrucciones(ctx->instrucciones, paquete);

    // Serializo Registros
    serializar_registros(&ctx->registros, paquete);
    
}

void serializar_instrucciones(t_list *instrucciones, t_paquete* paquete){
    int cant_instrucciones = list_size(instrucciones);
    for (int i =0; i<cant_instrucciones; i++){
        t_instruccion *instruccion = list_get(instrucciones, i);
        serializar_instruccion(instruccion, paquete);
    }
}

void serializar_instruccion(t_instruccion *instruccion, t_paquete *paquete) {

    int tamanio_parametro;
    

    agregar_a_paquete_dato_serializado(paquete, &(instruccion->operacion), sizeof(t_operacion));
    
    agregar_a_paquete_dato_serializado(paquete, &(instruccion->cantidad_parametros), sizeof(int));


    for (int i = 0; i < instruccion->cantidad_parametros; i++) {
        
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
