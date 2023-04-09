#include <init.h>

void inicializar_conexiones(int *conexion_memoria, t_config* config, t_log* logger) {
    char *ip_memoria = config_get_string_value(config, "IP_MEMORIA");
    char *puerto_memoria = config_get_string_value(config, "PUERTO_MEMORIA");


    *conexion_memoria = inicializar_cliente(ip_memoria, puerto_memoria, logger);
    
    char *puerto_escucha = config_get_string_value(config, "PUERTO_ESCUCHA");
    inicializar_servidor(IP, puerto_escucha, logger);
    
}