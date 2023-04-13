#include <init.h>


void inicializar_conexiones(int *conexion_cpu, int *conexion_memoria, int *conexion_filesystem, t_config* config, t_log* logger) {
    // CPU
    char *ip_cpu = config_get_string_value(config, "IP_CPU");
    char *puerto_cpu = config_get_string_value(config, "PUERTO_CPU");
    
    // FILESYSTEM
    char *ip_filesystem = config_get_string_value(config, "IP_FILESYSTEM");
    char *puerto_filesystem = config_get_string_value(config, "PUERTO_FILESYSTEM");
    
    // MEMORIA
    char *ip_memoria = config_get_string_value(config, "IP_MEMORIA");
    char *puerto_memoria = config_get_string_value(config, "PUERTO_MEMORIA");


    *conexion_cpu = inicializar_cliente(ip_cpu, puerto_cpu, logger);
    *conexion_memoria = inicializar_cliente(ip_memoria, puerto_memoria, logger);
    *conexion_filesystem = inicializar_cliente(ip_filesystem, puerto_filesystem, logger);


    
}