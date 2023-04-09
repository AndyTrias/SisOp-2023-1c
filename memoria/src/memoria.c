#include <memoria.h>

int main(int argc, char *argv[]) {

    if (argc != 2) {
        printf("Debe ingresar el archivo de configuracion\n");
        exit(1);
    }

    t_log* logger = iniciar_logger("./logs/memoria.log", "CONSOLA");
    t_config* config = iniciar_config("./config/memoria.config");
    
    char *puerto_escucha = config_get_string_value(config, "PUERTO_ESCUCHA");
    inicializar_servidor(IP, puerto_escucha, logger);

    terminar_programa(logger, config);

    return 0;
}
