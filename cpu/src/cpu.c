#include <utils/logs.h>
#include <utils/conexiones.h>
#include <utils/aux.h>

int main(int argc, char *argv[]) {

    if (argc != 2) {
        printf("Debe ingresar el archivo de configuracion\n");
        exit(1);
    }

    logger_y_config variables = inicializar_variables(argv[1], "./logs/cpu.log", "Cpu");
    
    t_log* logger = variables.logger;
    t_config* config = variables.config;

    char *puerto_escucha = config_get_string_value(config, "PUERTO_ESCUCHA");
    char *ip_memoria = config_get_string_value(config, "IP_MEMORIA");
    char *puerto_memoria = config_get_string_value(config, "PUERTO_MEMORIA");

    inicializar_servidor(IP, puerto_escucha, logger);

    int conexion_memoria = inicializar_cliente(ip_memoria, puerto_memoria, logger);

    terminar_programa(conexion_memoria, logger, config);

    return 0;
}

