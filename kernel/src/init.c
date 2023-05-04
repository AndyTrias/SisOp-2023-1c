#include <init.h>


void inicializar_conexiones(int *conexion_cpu, int *conexion_memoria, int *conexion_filesystem, t_config* config) {
    // CPU
    char *ip_cpu = config_get_string_value(config, "IP_CPU");
    char *puerto_cpu = config_get_string_value(config, "PUERTO_CPU");
    
    // FILESYSTEM
    char *ip_filesystem = config_get_string_value(config, "IP_FILESYSTEM");
    char *puerto_filesystem = config_get_string_value(config, "PUERTO_FILESYSTEM");
    
    // MEMORIA
    char *ip_memoria = config_get_string_value(config, "IP_MEMORIA");
    char *puerto_memoria = config_get_string_value(config, "PUERTO_MEMORIA");


    *conexion_cpu = inicializar_cliente(ip_cpu, puerto_cpu, LOGGER_KERNEL);
    *conexion_memoria = inicializar_cliente(ip_memoria, puerto_memoria, LOGGER_KERNEL);
    *conexion_filesystem = inicializar_cliente(ip_filesystem, puerto_filesystem, LOGGER_KERNEL);

}

void inicializar_variables_globales(t_config* config) {

    LOGGER_KERNEL = log_create("./logs/kernel.log", "KERNEL", 1, LOG_LEVEL_INFO);

    PID_COUNT = 0;

    LISTA_NEW = list_create();
    LISTA_READY = list_create();
    LISTA_EXEC = list_create();
    LISTA_BLOCK = list_create();

    ALGORITMO_PLANIFICACION = config_get_string_value(config, "ALGORITMO_PLANIFICACION");
    ESTIMACION_INICIAL = config_get_int_value(config, "ESTIMACION_INICIAL");
    HRRN_ALFA = config_get_double_value(config, "HRRN_ALFA");
    GRADO_MAX_MULTIPROGRAMACION = config_get_int_value(config, "GRADO_MAX_MULTIPROGRAMACION");
    RECURSOS = config_get_array_value(config, "RECURSOS");
    INSTANCIAS_RECURSOS = config_get_array_value(config, "INSTANCIAS_RECURSOS");

    //inicializacion de semaforos
    pthread_mutex_init(&MUTEX_LISTA_NEW, NULL);
    pthread_mutex_init(&MUTEX_LISTA_READY, NULL);
    sem_init(&CONSOLA_CONECTADA, 0, 0);
}