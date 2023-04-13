#include <file-system.h>

int main(int argc, char *argv[]) {
  
    if (argc != 2) {
    printf("Debe ingresar el archivo de configuracion\n");
    exit(1);
    }

  
    t_log* logger = iniciar_logger("./logs/file-system.log", "CONSOLA");
    t_config* config = iniciar_config("./config/file-system.config");
    
    int conexion_memoria;
    inicializar_conexiones(&conexion_memoria, config, logger);

    enviar_mensaje("Hola Memoria, soy el file-system", conexion_memoria);

    char *puerto_escucha = config_get_string_value(config, "PUERTO_ESCUCHA");
    inicializar_servidor(IP, puerto_escucha, logger);

    terminar_programa(logger, config);
    terminar_conexiones(1, conexion_memoria);

    return 0;
}