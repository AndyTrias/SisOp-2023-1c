#include <instruccionescpu.h>
// /*
// Instrucción Ejecutada: “PID: <PID> - Ejecutando: <INSTRUCCION> - <PARAMETROS>”   ***** Hecho *****
// Acceso Memoria: “PID: <PID> - Acción: <LEER / ESCRIBIR> - Segmento: <NUMERO SEGMENTO> - Dirección Física: <DIRECCION FISICA> - Valor: <VALOR LEIDO / ESCRITO>”
// Error Segmentation Fault: “PID: <PID> - Error SEG_FAULT- Segmento: <NUMERO SEGMENTO> - Offset: <OFFSET> - Tamaño: <TAMAÑO>”
// */




bool primera_instruccion = 0;

// //Cuando recibe un ctx

t_instruccion* fetch(t_ctx *ctx)
{
	t_instruccion* instruccion_nueva = list_get(ctx->instrucciones, ctx->program_counter); // Busca la instrucción y la guarda.
	log_info(LOGGER_CPU, "Program Counter: %d", ctx->program_counter);
	switch (instruccion_nueva->operacion)
	{
	case EXIT:
		break;
	default:
		ctx->program_counter++;
		break;
	}
	return instruccion_nueva;
}

void decode(t_instruccion* instruccion)
{
	switch (instruccion->operacion)
	{
	case SET:
		usleep(TIEMPO_RETARDO * 1000000);
		break;
	// case WRITE:
	// 	break;
	// case READ:
	// 	break;
	default:
		break;
	}
}

char* obtenerRegistro(t_registros* registros, const char* nombreRegistro)
{
    if (strcmp(nombreRegistro, "AX") == 0)
        return registros->AX;
    else if (strcmp(nombreRegistro, "BX") == 0)
        return registros->BX;
    else if (strcmp(nombreRegistro, "CX") == 0)
        return registros->CX;
    else if (strcmp(nombreRegistro, "DX") == 0)
        return registros->DX;
    else if (strcmp(nombreRegistro, "EAX") == 0)
        return registros->EAX;
    else if (strcmp(nombreRegistro, "EBX") == 0)
        return registros->EBX;
    else if (strcmp(nombreRegistro, "ECX") == 0)
        return registros->ECX;
    else if (strcmp(nombreRegistro, "EDX") == 0)
        return registros->EDX;
    else if (strcmp(nombreRegistro, "RAX") == 0)
        return registros->RAX;
    else if (strcmp(nombreRegistro, "RBX") == 0)
        return registros->RBX;
    else if (strcmp(nombreRegistro, "RCX") == 0)
        return registros->RCX;
    else if (strcmp(nombreRegistro, "RDX") == 0)
        return registros->RDX;
    else
        return NULL; // Si el nombre del registro no coincide, devuelve NULL o puedes manejarlo de otra manera según tus necesidades.
}

op_code execute(t_instruccion* instruccion_actual, t_ctx *ctx)
{
	switch (instruccion_actual->operacion)
	{
	case SET:
		log_info(LOGGER_CPU, "PID: %d  -Ejecutando: %d - %s %s", ctx->PID, instruccion_actual->operacion, instruccion_actual->parametros[0], instruccion_actual->parametros[1]);
		char* registro = obtenerRegistro(&ctx->registros, instruccion_actual->parametros[0]);
		strcpy(registro, instruccion_actual->parametros[1]);
		return 0;

	case MOV_IN:
		log_info(LOGGER_CPU, "PID: %d  -Ejecutando: %d - %s %s", ctx->PID, instruccion_actual->operacion, instruccion_actual->parametros[0], instruccion_actual->parametros[1]);
		agregar_parametro_desalojo(ctx, instruccion_actual->parametros[0]);
		void* dir_fisica = MMU(atoi(instruccion_actual->parametros[1]), atoi(instruccion_actual->parametros[0]), ctx);
		if (!dir_fisica){
			return SEG_FAULT;
		}
		agregar_parametro_desalojo(ctx, dir_fisica);
		t_paquete* paquete = crear_paquete(MOV_IN);
		serializar_motivos_desalojo(ctx->motivos_desalojo, paquete);
		enviar_paquete(paquete, SOCKET_MEMORIA);
		free(paquete);
		char *valor_leido = recibir_mensaje(SOCKET_MEMORIA);
		log_info(LOGGER_CPU, "PID: %d  -Acción: ESCRIBIR - Segmento: %s - Dirección Física: %s - Valor: %s", ctx->PID, floor_div(instruccion_actual->parametros[0], TAM_MAX_SEGMENTO/*TAMANIO_MAX_SEG*/), dir_fisica, valor_leido);
		//acceder a registro en instruccion_actual->parametros[1] y guardar valor_leido
		//ctx->registros.(instruccion_actual->parametros[1]) = valor_leido;
		return 0;

	case MOV_OUT:
		log_info(LOGGER_CPU, "PID: %d  -Ejecutando: %d - %s %s", ctx->PID, instruccion_actual->operacion, instruccion_actual->parametros[0], instruccion_actual->parametros[1]);
		char* mensaje_a_enviar = string_from_format("Escribi en la siguiente direccion de memoria %s el valor %s", instruccion_actual->parametros[0], obtenerRegistro(&ctx->registros, instruccion_actual->parametros[1]));
		dir_fisica = MMU(atoi(instruccion_actual->parametros[0]), atoi(instruccion_actual->parametros[1]), ctx);
		if (!dir_fisica){
			return SEG_FAULT;
		}
		//enviar_mensaje(mensaje_a_enviar, SOCKET_MEMORIA);
		//recibir ok memoria
		//log_info(LOGGER_CPU, "PID: %d  -Acción: LEER - Segmento: %s - Dirección Física: %p - Valor: %s", ctx->PID, floor_div(instruccion_actual->parametros[0], TAM_MAX_SEGMENTO/*TAMANIO_MAX_SEG*/), dir_fisica, mensaje_a_enviar);

		return 0;
	
	case WAIT:
		log_info(LOGGER_CPU, "PID: %d  -Ejecutando: %d - %s", ctx->PID, instruccion_actual->operacion, instruccion_actual->parametros[0]);
		agregar_parametro_desalojo(ctx, instruccion_actual->parametros[0]);
		return WAIT;
	
	case SIGNAL:
		log_info(LOGGER_CPU, "PID: %d  -Ejecutando: %d - %s", ctx->PID, instruccion_actual->operacion, instruccion_actual->parametros[0]);
		agregar_parametro_desalojo(ctx, instruccion_actual->parametros[0]);
		return SIGNAL;
	
	case YIELD:
		log_info(LOGGER_CPU, "PID: %d  -Ejecutando: %d", ctx->PID, instruccion_actual->operacion);
		return YIELD;

	case EXIT:
		log_info(LOGGER_CPU, "PID: %d  -Ejecutando: %d", ctx->PID, instruccion_actual->operacion);
		return EXIT;

	case IO:
		log_info(LOGGER_CPU, "PID: %d  -Ejecutando: %d - %s ", ctx->PID, instruccion_actual->operacion, instruccion_actual->parametros[0]);
		agregar_parametro_desalojo(ctx, instruccion_actual->parametros[0]);
		return IO;

	case F_OPEN:
		log_info(LOGGER_CPU, "PID: %d  -Ejecutando: %d - %s", ctx->PID, instruccion_actual->operacion, instruccion_actual->parametros[0]);
		agregar_parametro_desalojo(ctx, instruccion_actual->parametros[0]);
		return F_OPEN;

	case F_WRITE:
		log_info(LOGGER_CPU, "PID: %d  -Ejecutando: %d - %s %s", ctx->PID, instruccion_actual->operacion, instruccion_actual->parametros[0], instruccion_actual->parametros[1]);
		agregar_parametro_desalojo(ctx, instruccion_actual->parametros[0]);
		dir_fisica = MMU(atoi(instruccion_actual->parametros[1]), atoi(instruccion_actual->parametros[2]), ctx);
		if (!dir_fisica){
			return SEG_FAULT;
		}
		agregar_parametro_desalojo(ctx, dir_fisica);
		agregar_parametro_desalojo(ctx, instruccion_actual->parametros[2]);
		return F_READ;
	
	case F_READ:
		log_info(LOGGER_CPU, "PID: %d  -Ejecutando: %d - %s %s", ctx->PID, instruccion_actual->operacion, instruccion_actual->parametros[0], instruccion_actual->parametros[1]);
		agregar_parametro_desalojo(ctx, instruccion_actual->parametros[0]);
		dir_fisica = MMU(atoi(instruccion_actual->parametros[1]), atoi(instruccion_actual->parametros[2]), ctx);
		if (!dir_fisica){
			
			return SEG_FAULT;
		}
		agregar_parametro_desalojo(ctx, dir_fisica);
		agregar_parametro_desalojo(ctx, instruccion_actual->parametros[2]);
		return F_READ;
	
	case F_TRUNCATE:
		log_info(LOGGER_CPU, "PID: %d  -Ejecutando: %d - %s %s", ctx->PID, instruccion_actual->operacion, instruccion_actual->parametros[0], instruccion_actual->parametros[1]);
		agregar_parametro_desalojo(ctx, instruccion_actual->parametros[0]);
		agregar_parametro_desalojo(ctx, instruccion_actual->parametros[1]);
		return F_TRUNCATE;
	
	case F_CLOSE:
		log_info(LOGGER_CPU, "PID: %d  -Ejecutando: %d - %s", ctx->PID, instruccion_actual->operacion, instruccion_actual->parametros[0]);
		agregar_parametro_desalojo(ctx, instruccion_actual->parametros[0]);
		return F_CLOSE;

	case F_SEEK:
		log_info(LOGGER_CPU, "PID: %d  -Ejecutando: %d - %s %s", ctx->PID, instruccion_actual->operacion, instruccion_actual->parametros[0], instruccion_actual->parametros[1]);
		agregar_parametro_desalojo(ctx, instruccion_actual->parametros[0]);
		agregar_parametro_desalojo(ctx, instruccion_actual->parametros[1]);
		return F_SEEK;
	
	case CREATE_SEGMENT:
		log_info(LOGGER_CPU, "PID: %d  -Ejecutando: %d - %s %s", ctx->PID, instruccion_actual->operacion, instruccion_actual->parametros[0], instruccion_actual->parametros[1]);
		agregar_parametro_desalojo(ctx, instruccion_actual->parametros[0]);
		agregar_parametro_desalojo(ctx, instruccion_actual->parametros[1]);
		return CREATE_SEGMENT;
	
	case DELETE_SEGMENT:
		log_info(LOGGER_CPU, "PID: %d  -Ejecutando: %d - %s", ctx->PID, instruccion_actual->operacion, instruccion_actual->parametros[0]);
		agregar_parametro_desalojo(ctx, instruccion_actual->parametros[0]);
		return DELETE_SEGMENT;
	
	default:
		return 0;
	}
};

void* MMU(int direccion_logica, int bytes, t_ctx *ctx){
	int num_segmento = floor_div(direccion_logica, 128/*TAMANIO_MAX_SEG*/);
	int offset = direccion_logica % 128/*TAMANIO_MAX_SEG*/;

	if (offset + bytes > TAM_MAX_SEGMENTO/*TAMANIO_MAX_SEG*/){
		//“PID: <PID> - Error SEG_FAULT- Segmento: <NUMERO SEGMENTO> - Offset: <OFFSET> - Tamaño: <TAMAÑO>”
		log_error(LOGGER_CPU, "PID: %d - Error SEG_FAULT- Segmento: %d - Offset: %d - Tamaño: %d", ctx->PID, num_segmento, offset, bytes);
		return NULL;
	}

	t_segmento* segmento = list_get(ctx->tabla_segmentos, num_segmento);
	void* direccion_fisica = segmento->base + offset;
	return direccion_fisica;
}

int floor_div(int a, int b)
{
	return (a - (a % b)) / b;
}



void ciclo_de_instruccion(t_ctx *ctx)
{
	log_info(LOGGER_CPU, "Comenzando ciclo con nuevo CTX...");
	t_instruccion* instruccion_actual;

	while (ctx != NULL && ctx->program_counter <= ctx->cant_instrucciones)	{
		instruccion_actual = fetch(ctx);
		log_info(LOGGER_CPU, "Instruccion nº%d: %d", ctx->program_counter, instruccion_actual->operacion);
		decode(instruccion_actual);
		int cod_op = execute(instruccion_actual, ctx);

		// Devuelve 0 si debe seguir ejecutando
		// Devuelve un codigo_operacion si debe enviarlo al kernel
		// el codigo de operacion son los enums de enviar_paquete y son siempre > 0

		if (cod_op > 0)
		{
			t_paquete *paquete = crear_paquete(cod_op);
			serializar_contexto(ctx, paquete);
			enviar_paquete(paquete, SOCKET_KERNEL);
			free(paquete);
			ctx = NULL;
		}
	}
}
