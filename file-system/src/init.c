#include <init.h>

void inicializar_conexiones(int *conexion_memoria, t_config* config) {
    char *ip_memoria = config_get_string_value(config, "IP_MEMORIA");
    char *puerto_memoria = config_get_string_value(config, "PUERTO_MEMORIA");


    *conexion_memoria = inicializar_cliente(ip_memoria, puerto_memoria, LOGGER_FILE_SYSTEM);
    int *identificador = malloc(sizeof(int));
    *identificador = 0;
    send(*conexion_memoria, identificador, sizeof(int), 0);
    free(identificador);
    
}