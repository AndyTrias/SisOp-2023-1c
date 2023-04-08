#include <file-system.h>

int main(int argc, char *argv[]) {

    if (argc != 2) {
        printf("Debe ingresar el archivo de configuracion\n");
        exit(1);
    }

    logger_y_config variables = inicializar_variables(argv[1], "./logs/fileSystem.log", "FileSystem");
    
    t_log* logger = variables.logger;
    t_config* config = variables.config;

    char *puerto_escucha = config_get_string_value(config, "PUERTO_ESCUCHA");
    char *ip_memoria = config_get_string_value(config, "IP_MEMORIA");
    char *puerto_memoria = config_get_string_value(config, "PUERTO_MEMORIA");

    int conexion_memoria = inicializar_cliente(ip_memoria, puerto_memoria, logger);

    inicializar_servidor(IP, puerto_escucha, logger);

    terminar_programa(conexion_memoria, logger, config);

    return 0;
}