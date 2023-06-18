#include <init.h>

void inicializar_conexiones(int *conexion_memoria, t_config *config)
{
    char *ip_memoria = config_get_string_value(config, "IP_MEMORIA");
    char *puerto_memoria = config_get_string_value(config, "PUERTO_MEMORIA");

    *conexion_memoria = inicializar_cliente(ip_memoria, puerto_memoria, LOGGER_FILE_SYSTEM);
    int *identificador = malloc(sizeof(int));
    *identificador = 0;
    send(*conexion_memoria, identificador, sizeof(int), 0);
    free(identificador);
}

void inicializar_archivos(t_config *config)
{

    t_config *superbloque = levantar_superbloque(config);

    int block_count = config_get_int_value(superbloque, "BLOCK_COUNT");
    int block_size = config_get_int_value(superbloque, "BLOCK_SIZE");

    BITMAP_BLOQUES = levantar_bitmap(block_count, config_get_string_value(config, "PATH_BITMAP"));
    ARCHIVO_BLOQUES = levantar_bloques(block_count, block_size, config_get_string_value(config, "PATH_BLOQUES"));

    
}

t_bitarray *levantar_bitmap(int cantidad_bloques, char *path)
{

    // La cantidad de bits es la cantidad de bloques
    // int cantidad_de_bits = cantidad_bloques;
    // Cada bloque pesa un byte
    // Como cada byte son 8 bits -> la cantidad de bytes necesario es la cantidad de bloques dividido 8
    int cantidad_de_bytes_necesarios = cantidad_bloques / 8;
    bool archivo_recien_creado = false;

    FILE* file = abrir_o_crear_archivo(path, cantidad_de_bytes_necesarios, &archivo_recien_creado);

    void *memoria_bitmap = mmap(NULL, cantidad_de_bytes_necesarios, PROT_READ | PROT_WRITE, MAP_SHARED, fileno(file), 0);
    if (memoria_bitmap == MAP_FAILED)
    {
        log_error(LOGGER_FILE_SYSTEM, "Error al mapear el archivo del bitmap");
        fclose(file);
        abort();
    }

    t_bitarray *bitmap = bitarray_create_with_mode(memoria_bitmap, cantidad_de_bytes_necesarios, LSB_FIRST);

    // Si el bitmap se creo recien, se inicializa en 0
    if (archivo_recien_creado)
    {
        for (int i = 0; i < bitarray_get_max_bit(bitmap); i++)
        {
            bitarray_clean_bit(bitmap, i);
        }

        log_info(LOGGER_FILE_SYSTEM, "Se creo un archvo de bitmaps de %ld bits", bitarray_get_max_bit(bitmap));
    }

    return bitmap;
}

FILE *levantar_bloques(int tamaño_bloque, int cantidad_bloques, char *path)
{
    bool archivo_recien_creado = false;
    
    FILE* file = abrir_o_crear_archivo(path, tamaño_bloque * cantidad_bloques, &archivo_recien_creado);

    if (archivo_recien_creado)
    {
        log_info(LOGGER_FILE_SYSTEM, "Se creo un archivo de bloques de %d bytes", tamaño_bloque * cantidad_bloques);
    }

    return file;
}

t_config *levantar_superbloque(t_config *config)
{
    char *path_superbloque = config_get_string_value(config, "PATH_SUPERBLOQUE");
    log_info(LOGGER_FILE_SYSTEM, "Se levanto el superbloque");
    
    return iniciar_config(path_superbloque);

}


FILE *abrir_o_crear_archivo(char *path, int cantidad_de_bytes_necesarios, bool *archivo_recien_creado)
{
    FILE *file = fopen(path, "r+");

    // El archivo no existe, se crea y se inicializa
    if (file == NULL)
    {
        file = fopen(path, "w+");
        *archivo_recien_creado = true;
        if (file == NULL)
        {
            log_error(LOGGER_FILE_SYSTEM, "Error al crear el archivo");
            abort();
        }

        truncar_archivo(file, cantidad_de_bytes_necesarios);
    }

    return file;
}

void truncar_archivo(FILE *file, int tamaño)
{
    if (ftruncate(fileno(file), tamaño) == -1)
    {
        log_error(LOGGER_FILE_SYSTEM, "Error al ajustar el tamaño del archivo");
        fclose(file);
        abort();
    }
}