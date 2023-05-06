#include <init.h>

void inicializar_conexiones(int *conexion_memoria, t_config* config, t_log* logger) {
    char *ip_memoria = config_get_string_value(config, "IP_MEMORIA");
    char *puerto_memoria = config_get_string_value(config, "PUERTO_MEMORIA");


    *conexion_memoria = inicializar_cliente(ip_memoria, puerto_memoria, logger);
}

void inicializar_variables_globales(t_config* config) {

    TIEMPO_RETARDO = atoi(config_get_string_value(config, "RETARDO_INSTRUCCION"));

}