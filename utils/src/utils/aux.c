#include "aux.h"

void terminar_programa(t_log* logger, t_config* config)
{
	/* Y por ultimo, hay que liberar lo que utilizamos (conexion, log y config) 
	  con las funciones de las commons y del TP mencionadas en el enunciado */

	if (logger) {
		log_destroy(logger);
	}

	if (config){
		config_destroy(config);
	}
}


