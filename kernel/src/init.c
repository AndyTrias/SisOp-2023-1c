#include <init.h>

void inicializar_variables_globales(t_config* config) {

    LOGGER_KERNEL = log_create("./logs/kernel.log", "KERNEL", 1, LOG_LEVEL_INFO);

    PID_COUNT = 0;

    ALGORITMO_PLANIFICACION = config_get_string_value(config, "ALGORITMO_PLANIFICACION");
    ESTIMACION_INICIAL = config_get_int_value(config, "ESTIMACION_INICIAL");
    HRRN_ALFA = config_get_double_value(config, "HRRN_ALFA");
    GRADO_MAX_MULTIPROGRAMACION = config_get_int_value(config, "GRADO_MAX_MULTIPROGRAMACION");
    RECURSOS = config_get_array_value(config, "RECURSOS");
    INSTANCIAS_RECURSOS = config_get_array_value(config, "INSTANCIAS_RECURSOS");
    
    inicializar_estados();

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


void inicializar_estados(){
    LISTA_NEW = list_create();
    LISTA_READY = list_create();
    EJECUTANDO = NULL;
    LISTAS_BLOCK = list_create();
    TIEMPO_CORRIENDO = temporal_create();
    
    // Creo las listas de blocked
    int i;
    for (i = 0; i < string_array_size(RECURSOS); i++) {
        t_list* aux = list_create();
        list_add(LISTAS_BLOCK,aux);
    }
}

void inicializar_conexiones(t_config* config) {
    // CPU
    char *ip_cpu = config_get_string_value(config, "IP_CPU");
    char *puerto_cpu = config_get_string_value(config, "PUERTO_CPU");
    
    // FILESYSTEM
    char *ip_filesystem = config_get_string_value(config, "IP_FILESYSTEM");
    char *puerto_filesystem = config_get_string_value(config, "PUERTO_FILESYSTEM");
    
    // MEMORIA
    char *ip_memoria = config_get_string_value(config, "IP_MEMORIA");
    char *puerto_memoria = config_get_string_value(config, "PUERTO_MEMORIA");

    SOCKET_CPU = inicializar_cliente(ip_cpu, puerto_cpu, LOGGER_KERNEL);
    SOCKET_MEMORIA = inicializar_cliente(ip_memoria, puerto_memoria, LOGGER_KERNEL);
    SOCKET_FILESYSTEM = inicializar_cliente(ip_filesystem, puerto_filesystem, LOGGER_KERNEL);

    int *identificador = malloc(sizeof(int));
    *identificador = 1;
    send(SOCKET_MEMORIA, identificador, sizeof(int), 0);
    free(identificador);
}
