#include <utils/logs.h>
#include <utils/conexiones.h>
#include <utils/aux.h>

int main(int argc, char *argv[]) {

    if (argc != 2) {
        printf("Debe ingresar el archivo de configuracion\n");
        exit(1);
    }

    logger_y_config variables = inicializar_variables(argv[1], "./logs/memoria.log", "Memoria");
    
    t_log* logger = variables.logger;
    t_config* config = variables.config;

    char *puerto_escucha = config_get_string_value(config, "PUERTO_ESCUCHA");

    inicializar_servidor(IP, puerto_escucha, logger);

    return 0;
}
