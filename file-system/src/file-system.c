#include <file-system.h>

int main(int argc, char *argv[]) {
  
    LOGGER_FILE_SYSTEM = iniciar_logger("./logs/file-system.log", "CONSOLA");

    if (argc != 2) {
        log_error(LOGGER_FILE_SYSTEM, "Debe ingresar el archivo de configuracion\n");
        return EXIT_FAILURE;
    }

    t_config* config = iniciar_config(argv[1]);

    // meter en otra funcion
    char* path_superbloque = config_get_string_value(config, "PATH_SUPERBLOQUE");
    t_config* config_superbloque = iniciar_config(path_superbloque);
    
    log_info(LOGGER_FILE_SYSTEM, "Tamanio del bloque: %d", config_get_int_value(config_superbloque, "BLOCK_SIZE"));
    log_info(LOGGER_FILE_SYSTEM, "Cantidad de bloques: %d", config_get_int_value(config_superbloque, "BLOCK_COUNT"));

    char* path_bitmap = config_get_string_value(config, "PATH_BITMAP");
    int block_count = config_get_int_value(config_superbloque, "BLOCK_COUNT");
    
    FILE* file = fopen(path_bitmap, "w");
    if (!file) {
        log_error(LOGGER_FILE_SYSTEM, "Error al abrir el archivo");
        return EXIT_FAILURE;
    }

    // Establecer el tamaño del archivo
    if (ftruncate(fileno(file), block_count) == -1) {
        log_error(LOGGER_FILE_SYSTEM, "Error al establecer el tamaño del archivo");
        return EXIT_FAILURE;
    }

    // Mapear el archivo a la memoria
    void* mapped_data = mmap(NULL, block_count, PROT_READ | PROT_WRITE, MAP_SHARED, fileno(file), 0);
    if (mapped_data == MAP_FAILED) {
        log_error(LOGGER_FILE_SYSTEM, "Error al mapear el archivo a la memoria");
        return EXIT_FAILURE;
    }

    // Crear el bitmap
    t_bitarray* bitmap = bitarray_create_with_mode(mapped_data, block_count, LSB_FIRST);

    // Establecer todos los bits en 0
    for (int i = 0; i < block_count; i++) {
        bitarray_clean_bit(bitmap, i);
    }
    
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