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

    SOCKET_CPU = *conexion_cpu;

}

void inicializar_listas_blocked(){
    int i;
    for (i = 0; i < sizeof(RECURSOS); i++) {
        list_add(LISTAS_BLOCK,list_create());
    }
}

void inicializar_variables_globales(t_config* config) {

    LOGGER_KERNEL = log_create("./logs/kernel.log", "KERNEL", 1, LOG_LEVEL_INFO);

    PID_COUNT = 0;

    LISTA_NEW = list_create();
    LISTA_READY = list_create();
    EJECUTANDO = NULL;
    //inicializar LISTAS_BLOCKED con tantas listas como recursos haya en el archivo de configuracion
    inicializar_listas_blocked();



    ALGORITMO_PLANIFICACION = config_get_string_value(config, "ALGORITMO_PLANIFICACION");
    ESTIMACION_INICIAL = config_get_int_value(config, "ESTIMACION_INICIAL");
    HRRN_ALFA = config_get_double_value(config, "HRRN_ALFA");
    GRADO_MAX_MULTIPROGRAMACION = config_get_int_value(config, "GRADO_MAX_MULTIPROGRAMACION");
    RECURSOS = config_get_array_value(config, "RECURSOS");
    INSTANCIAS_RECURSOS = config_get_array_value(config, "INSTANCIAS_RECURSOS");

    //inicializacion de semaforos
    inicializar_semaforos();
}



void inicializar_semaforos() {
    pthread_mutex_init(&MUTEX_LISTA_NEW, NULL);
    pthread_mutex_init(&MUTEX_LISTA_READY, NULL);
    pthread_mutex_init(&MUTEX_LISTA_BLOCK, NULL);
    sem_init(&PROCESO_EN_NEW, 0, 0);
    sem_init(&PROCESO_EN_READY, 0, 0);
    sem_init(&PROCESO_EN_BLOCK, 0, 0);
    sem_init(&GRADO_MULTIPROGRAMACION, 0, GRADO_MAX_MULTIPROGRAMACION);
    sem_init(&CORTO_PLAZO, 0, 0);

}