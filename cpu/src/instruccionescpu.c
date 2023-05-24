#include <instruccionescpu.h>
// /*
// Instrucción Ejecutada: “PID: <PID> - Ejecutando: <INSTRUCCION> - <PARAMETROS>”   ***** Hecho *****
// Acceso Memoria: “PID: <PID> - Acción: <LEER / ESCRIBIR> - Segmento: <NUMERO SEGMENTO> - Dirección Física: <DIRECCION FISICA> - Valor: <VALOR LEIDO / ESCRITO>”
// Error Segmentation Fault: “PID: <PID> - Error SEG_FAULT- Segmento: <NUMERO SEGMENTO> - Offset: <OFFSET> - Tamaño: <TAMAÑO>”
// */

bool proceso_terminado;
bool volver_a_ready;

// //Cuando recibe un ctx
t_instruccion* fetch(t_ctx *ctx)
{
	t_instruccion* instruccion_nueva;
	instruccion_nueva = list_get(ctx->instrucciones, ctx->program_counter); // Busca la instrucción y la guarda.
	if()
	ctx->program_counter++;
	log_info(LOGGER_CPU, "Program Counter: %d", ctx->program_counter);
	return instruccion_nueva;
}

void decode(t_instruccion* instruccion, int retraso)
{
	if (instruccion->operacion == SET)
	{
		log_info(LOGGER_CPU, "Una siesta rápida");
		usleep(retraso * 1000);
		log_info(LOGGER_CPU, "We don't have time for this");
	}
}

int comparar_letra(char *letra)
{
	int i = 0;
	int j = strlen(letra) - 2;
	char *comparador = "ABCD";
	while (i != 4)
	{
		if (comparador[i] == letra[j])
		{
			break;
		};
		i++;
	};

	return i;
}

int devolver_num_registro(char *original)
{
	int k = 0;
	char nombre_registro[strlen(original) - 1];
	strcpy(nombre_registro, original);

	if (strlen(nombre_registro) == 3)
	{
		k = comparar_letra((nombre_registro));

		if (string_starts_with(nombre_registro, "R"))
		{
			k = 8 + k;
		};

		if (string_starts_with(nombre_registro, "E"))
		{
			k = 4 + k;
		};
	};

	if (strlen(nombre_registro) == 2)
	{
		k = comparar_letra(nombre_registro);
	};
	return k;
}

op_code execute(t_instruccion* instruccion_actual, t_ctx *ctx)
{
	switch (instruccion_actual->operacion)
	{
	case SET:
		log_info(LOGGER_CPU, "PID: %d  -Ejecutando: %d - %s %s", ctx->PID, instruccion_actual->operacion, instruccion_actual->parametros[0], instruccion_actual->parametros[1]);
		switch (devolver_num_registro(instruccion_actual->parametros[0]))
		{
		case 0:
			strcpy(ctx->registros.AX, instruccion_actual->parametros[1]);
			log_info(LOGGER_CPU, "Num %s", ctx->registros.AX);
			break;
		case 1:
			strcpy(ctx->registros.BX, instruccion_actual->parametros[1]);
			break;
		case 2:
			strcpy(ctx->registros.CX, instruccion_actual->parametros[1]);
			break;
		case 3:
			strcpy(ctx->registros.DX, instruccion_actual->parametros[1]);
			break;
		case 4:
			strcpy(ctx->registros.EAX, instruccion_actual->parametros[1]);
			break;
		case 5:
			strcpy(ctx->registros.EBX, instruccion_actual->parametros[1]);
			break;
		case 6:
			strcpy(ctx->registros.ECX, instruccion_actual->parametros[1]);
			break;
		case 7:
			strcpy(ctx->registros.EDX, instruccion_actual->parametros[1]);
			break;
		case 8:
			strcpy(ctx->registros.RAX, instruccion_actual->parametros[1]);
			break;
		case 9:
			strcpy(ctx->registros.RBX, instruccion_actual->parametros[1]);
			break;
		case 10:
			strcpy(ctx->registros.RCX, instruccion_actual->parametros[1]);
			break;
		case 11:
			strcpy(ctx->registros.RDX, instruccion_actual->parametros[1]);
			break;
		default:
			break;
		}

		return 0;

	case YIELD:
		log_info(LOGGER_CPU, "PID: %d  -Ejecutando: %d", ctx->PID, instruccion_actual->operacion);
		return DESALOJAR;

	case EXIT:
		log_info(LOGGER_CPU, "PID: %d  -Ejecutando: %d", ctx->PID, instruccion_actual->operacion);
		return TERMINAR;

	default:
		return 0;
	}
};

void ciclo_de_instruccion(t_ctx *ctx)
{
	log_info(LOGGER_CPU, "Comenzando ciclo con nuevo CTX...");
	t_instruccion* instruccion_actual;

	while (ctx != NULL && ctx->program_counter <= ctx->cant_instrucciones)	{
		instruccion_actual = fetch(ctx);
		log_info(LOGGER_CPU, "Instruccion nº%d: %d", ctx->program_counter, instruccion_actual->operacion);
		decode(instruccion_actual, TIEMPO_RETARDO);
		int cod_op = execute(instruccion_actual, ctx);

		// Devuelve 0 si debe seguire ejecutando
		// Devuelve un codigo_operacion si debe enviarlo al kernel
		// el codigo de operacion son los enums de enviar_paquete y son siempre > 0

		if (cod_op > 0)
		{
			t_paquete *paquete = crear_paquete(cod_op);
			serializar_contexto(ctx, paquete);
			enviar_paquete(paquete, SOCKET_KERNEL);
			ctx = NULL;
		}
	}
}
