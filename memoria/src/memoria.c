#include <memoria.h>

int main(int argc, char *argv[]) {

    if (argc != 2) {
        printf("Debe ingresar el archivo de configuracion\n");
        exit(1);
    }

    logger_memoria = iniciar_logger("./logs/memoria.log", "MEMORIA");
    t_config* config = iniciar_config(argv[1]);
    
    char *puerto_escucha = config_get_string_value(config, "PUERTO_ESCUCHA");
    int socket_servidor = crear_servidor(IP, puerto_escucha);
    conectar_modulos(socket_servidor);

    terminar_programa(logger_memoria, config);

    return 0;
}
