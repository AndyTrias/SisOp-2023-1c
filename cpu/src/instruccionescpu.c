#include <instruccionescpu.h>
// /*
// Instrucción Ejecutada: “PID: <PID> - Ejecutando: <INSTRUCCION> - <PARAMETROS>”   ***** Hecho *****
// Acceso Memoria: “PID: <PID> - Acción: <LEER / ESCRIBIR> - Segmento: <NUMERO SEGMENTO> - Dirección Física: <DIRECCION FISICA> - Valor: <VALOR LEIDO / ESCRITO>”
// Error Segmentation Fault: “PID: <PID> - Error SEG_FAULT- Segmento: <NUMERO SEGMENTO> - Offset: <OFFSET> - Tamaño: <TAMAÑO>”
// */

bool proceso_terminado;
bool volver_a_ready;
    
// //Cuando recibe un ctx
t_instruccion fetch(t_ctx* ctx) {
 	t_instruccion instruccion_nueva;
 	instruccion_nueva = *((t_instruccion*) list_get(ctx->instrucciones, ctx->program_counter)); //Busca la instrucción y la guarda.
 	log_info(LOGGER_CPU, "Program Counter: %d + 1", ctx->program_counter);
 	ctx->program_counter++;
 	return instruccion_nueva;
};

void decode(t_instruccion instruccion, int retraso) {
	if (instruccion.operacion == SET){
		log_info(LOGGER_CPU, "Una siesta rápida");
 		usleep(retraso*1000);
		log_info(LOGGER_CPU, "We don't have time for this");
	}
};

/*
int comparar_letra(char * letra){
	int i=0;
	char * comparador = "ABCD";
	while(i != 4){
		if(comparador[i] == letra){
			
			break;
		};
		i++;
	};
	return i;
};


int devolver_num_registro(char * nombre_registro){
 	int aux;
	if (strlen(nombre_registro) == 3){
		int i = comparar_letra((nombre_registro[1]));

		if(nombre_registro[0] == "R"){
			aux = 9+i;
		};

		if(nombre_registro[0] == "E"){
			aux = 5+i;
		};
	};
	if (strlen(nombre_registro) == 2){
		aux = comparar_letra(nombre_registro[0]);
	};
 	return aux;
};

*/

void execute(const t_instruccion instruccion_actual, t_ctx* ctx) {
	switch (instruccion_actual.operacion) {
	case SET:
		log_info(LOGGER_CPU, "PID: %d  -Ejecutando: %d - %s %s", ctx->PID, instruccion_actual.operacion, instruccion_actual.parametros[0], instruccion_actual.parametros[1]); 
		// ctx->registros[devolver_num_registro(instruccion_actual.parametros[0])] = instruccion_actual.parametros[1];
		// log_info(LOGGER_CPU, "Num %d", devolver_num_registro(instruccion_actual.parametros[0]));
 		break;
 	case YIELD:
		log_info(LOGGER_CPU, "PID: %d  -Ejecutando: %d", ctx->PID, instruccion_actual.operacion); 
 		proceso_terminado = true;
		// vuelve a ready
 		// bool volver_a_ready = true;
 	case EXIT:
		log_info(LOGGER_CPU, "PID: %d  -Ejecutando: %d", ctx->PID, instruccion_actual.operacion); 
 		proceso_terminado = true;
 		break;
 	default:
 		break;
 	}
};

void ciclo_de_instruccion(t_ctx* ctx) {
    log_info(LOGGER_CPU, "Comenzando ciclo con nuevo PCB...");
	t_instruccion instruccion_actual;
    t_buffer* buffer = malloc(sizeof(t_buffer));

 	while (ctx != NULL) {
 		instruccion_actual = fetch(ctx);
 		log_info(LOGGER_CPU, "Instruccion nº%d: %d", ctx->program_counter, instruccion_actual.operacion);
		decode(instruccion_actual, TIEMPO_RETARDO);
    	execute(instruccion_actual, ctx);
// 		if (proceso_terminado) {
// 			proceso_terminado = false;
// 			log_info(cpu_log, "Proceso %d TERMINADO", ctx->PID);
// 			log_info(cpu_log, "Devolviendo PCB actualizado del PID %d...", ctx->PID);
// 			/*
// 			enviar_pcb(ctx, socket_dispatch, 0);									//
// 			if (vover_a_ready){
// 				volver_a_ready = false
// 				enviar_socket("YIELD")	
// 			}
// 			envira_socket("EXIT")
// 			liberar_pcb(ctx);														//
// 			*/
 			ctx = NULL;
//			continue;
 		}
		free(buffer);
 	}

// }



		


