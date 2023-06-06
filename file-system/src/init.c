#include <init.h>

void inicializar_conexiones(int *conexion_memoria, t_config* config) {
    char *ip_memoria = config_get_string_value(config, "IP_MEMORIA");
    char *puerto_memoria = config_get_string_value(config, "PUERTO_MEMORIA");


    *conexion_memoria = inicializar_cliente(ip_memoria, puerto_memoria, LOGGER_FILE_SYSTEM);
    int *identificador = malloc(sizeof(int));
    *identificador = 0;
    send(*conexion_memoria, identificador, sizeof(int), 0);
    free(identificador);
    
}

void inicializar_archivos(t_config* config) {

    t_config* superbloque = levantar_superbloque(config);

    int block_count = config_get_int_value(superbloque, "BLOCK_COUNT");
    // int block_size = config_get_int_value(superbloque, "BLOCK_SIZE");
    
    BITMAP_BLOQUES = levantar_bitmap(block_count, config_get_string_value(config, "PATH_BITMAP"));
    // ARCHIVO_BLOQUES = levantar_bloques(superbloque);
}


t_bitarray* levantar_bitmap(int cantidad_bloques, char* path) {
    int bits_bitmap = cantidad_bloques / 8;
    
    int fd = open(path, O_RDWR);
    if (fd == -1) {
        log_error(LOGGER_FILE_SYSTEM, "Error al abrir el archivo del bitmap");
        abort();
    }

    if (ftruncate(fd, bits_bitmap) == -1) {
        log_error(LOGGER_FILE_SYSTEM, "Error al ajustar el tamaño del archivo del bitmap");
        close(fd);
        abort();
    }

    void* memoria_bitmap = mmap(NULL, bits_bitmap, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (memoria_bitmap == MAP_FAILED) {
        log_error(LOGGER_FILE_SYSTEM, "Error al mapear el archivo del bitmap");
        close(fd);
        abort();
    }


    t_bitarray* bitmap = bitarray_create_with_mode(memoria_bitmap, bits_bitmap, LSB_FIRST);

    for (int i = 0; i < bitarray_get_max_bit(bitmap); i++) {
        bitarray_clean_bit(bitmap, i);
    }

    log_info(LOGGER_FILE_SYSTEM, "Se creo un bitmap de %ld bits", bitarray_get_max_bit(bitmap));

    return bitmap;
}

t_config* levantar_superbloque(t_config* config) {
    char* path_superbloque = config_get_string_value(config, "PATH_SUPERBLOQUE");
    return iniciar_config(path_superbloque);

    log_info(LOGGER_FILE_SYSTEM, "Se levanto el superbloque");
}
    