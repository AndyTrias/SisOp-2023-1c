#include <cpu.h>

int main(int argc, char *argv[]) {
  
    if (argc != 2) {
        printf("Debe ingresar el archivo de configuracion\n");
        exit(1);
    }


    LOGGER_CPU = iniciar_logger("./logs/cpu.log", "CPU");
    t_config* config = iniciar_config(argv[1]);
    
    int conexion_memoria;
    inicializar_conexiones(&conexion_memoria, config, LOGGER_CPU);

    char *puerto_escucha = config_get_string_value(config, "PUERTO_ESCUCHA");
    int socket_servidor = crear_servidor(IP, puerto_escucha);

    
    conectar_kernel(socket_servidor);
    
    /*
    log_info(cpu_log,"Esperando la conexion de interrupcion...");
    log_info(cpu_log,"Todo listo, inicializando...");
    log_info(cpu_log,"No hay CTX, esperando...");
    while(true){
        recibir_paquete_kernel(int);
        log_info(cpu_log,"Llego el CTX");
        ctx = deserealizar_instruccion(void*, int* );
        log_info(cpu_log,"CTX obtenido!!!");
        ciclo_de_instruccion(ctx);
    }
    free(buffer);
    */

    terminar_programa(LOGGER_CPU, config);
    terminar_conexiones(1, conexion_memoria);

    return 0;
}

