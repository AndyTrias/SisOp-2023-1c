#include <file-system.h>

int main(int argc, char *argv[]) {
  
    LOGGER_FILE_SYSTEM = iniciar_logger("./logs/file-system.log", "CONSOLA");

    if (argc != 2) {
        log_error(LOGGER_FILE_SYSTEM, "Debe ingresar el archivo de configuracion\n");
        return EXIT_FAILURE;
    }

    t_config* config = iniciar_config(argv[1]);

    inicializar_archivos(config);

    
    int conexion_memoria;
    inicializar_conexiones(&conexion_memoria, config);

    enviar_mensaje("Hola Memoria, soy el file-system", conexion_memoria);

    char *puerto_escucha = config_get_string_value(config, "PUERTO_ESCUCHA");
    int socket_servidor = crear_servidor(IP, puerto_escucha);

    conectar_kernel(socket_servidor);

    terminar_programa(LOGGER_FILE_SYSTEM, config);
    terminar_conexiones(1, conexion_memoria);

    return 0;
}