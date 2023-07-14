#include <cpu.h>

int main(int argc, char *argv[]) {
  
    if (argc != 2) {
        printf("Debe ingresar el archivo de configuracion\n");
        exit(1);
    }


    t_config* config = iniciar_config(argv[1]);
<<<<<<< HEAD
    char* IP = "127.0.0.1";
=======
    // t_config* configIp = iniciar_config("/home/utnso/Desktop/tp-2023-1c-Los-pibardos-de-operativos/utils/src/utils/config/ips.config");
>>>>>>> ff92ce968b05f7ea8d61cb3fc5f94e962a061b1e
    inicializar_variables_globales(config);
    
    int conexion_memoria;
    inicializar_conexiones(&conexion_memoria, config, LOGGER_CPU);

    char *puerto_escucha = config_get_string_value(config, "PUERTO_ESCUCHA");
<<<<<<< HEAD
    int socket_servidor = crear_servidor(IP, puerto_escucha);
=======
    char* ip = config_get_string_value(config, "IP_CPU");
    int socket_servidor = crear_servidor(ip, puerto_escucha);
>>>>>>> ff92ce968b05f7ea8d61cb3fc5f94e962a061b1e

    
    conectar_kernel(socket_servidor);
    

    terminar_programa(LOGGER_CPU, config);
    terminar_conexiones(1, conexion_memoria);

    return 0;
}

