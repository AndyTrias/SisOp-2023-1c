#include "aux.h"

void terminar_programa(int conexion, t_log* logger, t_config* config)
{
	/* Y por ultimo, hay que liberar lo que utilizamos (conexion, log y config) 
	  con las funciones de las commons y del TP mencionadas en el enunciado */

	if (logger) {
		log_destroy(logger);
	}

	if (config){
		config_destroy(config);
	}

	if (conexion) {
		close(conexion);
	}
}

logger_y_config inicializar_variables(char* ruta_config, char* ruta_log, char* nombre_programa)
{
	t_config* config = iniciar_config(ruta_config);
	t_log* logger = iniciar_logger(ruta_log, nombre_programa);

	logger_y_config variables = {logger, config};

	return variables;
}

void inicializar_servidor(char* ip, char* puerto, t_log* logger)
{
	int socket_servidor = iniciar_servidor(ip, puerto);

	if (socket_servidor == -1) {
		log_error(logger, "No se pudo iniciar el servidor");
		exit(1);
	}

	log_info(logger, "Servidor iniciado en %s:%s", ip, puerto);

	while (1) {
		int socket_cliente = esperar_cliente(socket_servidor);

		if (socket_cliente == -1) {
			log_error(logger, "No se pudo aceptar el cliente");
			exit(1);
		}

		log_info(logger, "Se conecto un cliente");
	}
}

int inicializar_cliente(char* ip, char* puerto, t_log* logger)
{
	int socket_cliente = crear_conexion(ip, puerto);

	if (socket_cliente == -1) {
		log_error(logger, "No se pudo conectar al servidor %s:%s", ip, puerto);
		return -1;
	}

	log_info(logger, "Se conecto al servidor %s:%s", ip, puerto);

	return socket_cliente;
}