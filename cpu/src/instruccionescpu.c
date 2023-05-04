#include <instruccionescpu.h>
/*
Instrucción Ejecutada: “PID: <PID> - Ejecutando: <INSTRUCCION> - <PARAMETROS>”   ***** Hecho *****
Acceso Memoria: “PID: <PID> - Acción: <LEER / ESCRIBIR> - Segmento: <NUMERO SEGMENTO> - Dirección Física: <DIRECCION FISICA> - Valor: <VALOR LEIDO / ESCRITO>”
Error Segmentation Fault: “PID: <PID> - Error SEG_FAULT- Segmento: <NUMERO SEGMENTO> - Offset: <OFFSET> - Tamaño: <TAMAÑO>”
*/

bool proceso_terminado;
bool volver_a_ready;
    
//Cuando recibe un ctx

void ciclo_de_instruccion(t_ctx* ctx) {
	log_info(cpu_log, "Comenzando ciclo con nuevo PCB...");
	t_instruccion instruccion_actual;
	t_buffer* buffer = malloc(sizeof(t_buffer));

	while (ctx) {
		instruccion_actual = fetch(ctx);
        log_info(cpu_log, "PID: %d  -Ejecutando: %d - %d", ctx->PID, instruccion_actual->identificador, instruccion_actual->parametro[0]); //Primer Log obligatorio
		log_info(cpu_log, "Instruccion nº%d: %d", ctx->program_counter, instruccion_actual.identificador);
		
        int TIEMPO_RETARDO = config_get_string_value(config, "RETARDO_INSTRUCCION"); // lo más probable es que no funcione
		
        if (decode(instruccion_actual)){
			usleep(TIEMPO_RETARDO*1000);
        }
		
        execute(instruccion_actual, ctx);
		if (proceso_terminado) {
			proceso_terminado = false;
			log_info(cpu_log, "Proceso %d TERMINADO", ctx->PID);
			log_info(cpu_log, "Devolviendo PCB actualizado del PID %d...", ctx->PID);
			/*
			enviar_pcb(ctx, socket_dispatch, 0);									//
			liberar_pcb(ctx);														//
			*/
			ctx = NULL;
			continue;
		}
	}
	free(buffer);

}

t_instruccion fetch(t_ctx* ctx) {
	t_instruccion instruccion_nueva;
	instruccion_nueva = *((t_instruccion*) list_get(ctx->instrucciones, ctx->program_counter)); //Busca la instrucción y la guarda.
	log_info(cpu_log, "Program Counter: %d + 1", ctx->program_counter);
	ctx->program_counter++;
	return instruccion_nueva;
}

bool decode(t_instruccion instruccion) {
	if (instruccion.operacion == SET)
		return true;
	else
		return false;
}

void execute(const t_instruccion instruccion, t_ctx ctx) {
	switch (instruccion.operacion) {
	case SET:
		char * nombre_registro = Devolver_registro(ctx->registros ,instruccion.parametros[0])
		registro_registro = instruccion.parametros[1];
		break;
	case YIELD:
		proceso_terminado = true;
		log_info(cpu_log, "Tiene que volver a la cola del ready"); // Tiene que volver a Ready
		bool volver_a_ready = true; // Sería con un bool??
	case EXIT:
		proceso_terminado = true;
		break;
	default:
		break;
		}
	return 0;
}

char * Devolver_registro(t_registros registro, char * nombre_registro){
	registro.nombre_registro 
	return ;
}