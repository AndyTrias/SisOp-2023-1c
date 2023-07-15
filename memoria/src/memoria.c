#include <memoria.h>

int main(int argc, char *argv[]) {

    if (argc != 2) {
        printf("Debe ingresar el archivo de configuracion\n");
        exit(1);
    }

    LOGGER_MEMORIA = iniciar_logger("./logs/memoria.log", "MEMORIA");
    t_config* config = iniciar_config(argv[1]);
    // t_config* configIp = iniciar_config("/home/utnso/Desktop/tp-2023-1c-Los-pibardos-de-operativos/utils/src/utils/config/ips.config");
    iniciar_estructuras(config);
    
    char *puerto_escucha = config_get_string_value(config, "PUERTO_ESCUCHA");
    char* ip = config_get_string_value(config, "IP_MEMORIA");
    int socket_servidor = crear_servidor(ip, puerto_escucha);
    conectar_modulos(socket_servidor);

    terminar_programa(LOGGER_MEMORIA, config);

    return 0;
}
