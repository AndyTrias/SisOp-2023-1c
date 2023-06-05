#include "estructuras.h"

void* memoria_principal;

void iniciar_estructuras(t_config* config){
    int tam_memoria = config_get_int_value(config, "TAM_MEMORIA");
    memoria_principal = malloc(tam_memoria);
    log_info(LOGGER_MEMORIA, "Se creo el espacio contiguo de memoria");
}