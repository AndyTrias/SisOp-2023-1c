#include <fcb.h>

char *path_fcb;

void levantar_diccionario_fcb(t_config *config)
{
    path_fcb = config_get_string_value(config, "PATH_FCB");

    DICCIONARIO_FCB = dictionary_create();
    DIR *directorio = opendir(path_fcb);
    if (directorio == NULL)
    {
        log_error(LOGGER_FILE_SYSTEM, "Error al abrir el directorio de FCB.\n");
        abort();
    }

    struct dirent *archivo;
    char *nombre_archivo, *ruta_archivo;
    while ((archivo = readdir(directorio)) != NULL)
    {
        if (archivo->d_type == DT_REG)
        {
            // Solo procesar archivos regulares (ignorar directorios y otros tipos)
            ruta_archivo = string_from_format("%s/%s", path_fcb, archivo->d_name);
            nombre_archivo = string_from_format("%s", archivo->d_name);

            t_config *config_archivo = iniciar_config(ruta_archivo);
            dictionary_put(DICCIONARIO_FCB, nombre_archivo, config_archivo);

            // Realizar operaciones con el archivo
            log_info(LOGGER_FILE_SYSTEM, "Archivo encontrado: %s\n", ruta_archivo);
        }
    }
    closedir(directorio);
    free(nombre_archivo);
    free(ruta_archivo);
    free(archivo);
}

void crear_fcb(char *nombre_archivo)
{

    if (dictionary_has_key(DICCIONARIO_FCB, nombre_archivo))
    {
        log_error(LOGGER_FILE_SYSTEM, "El archivo %s ya existe.\n", nombre_archivo);
    }

    else
    {
        char *path_archivo = string_from_format("%s/%s", path_fcb, nombre_archivo);

        t_config *nuevo = iniciar_config(path_archivo);

        config_set_value(nuevo, "NOMBRE_ARCHIVO", nombre_archivo);
        config_set_value(nuevo, "TAMANIO_ARCHIVO", "0");

        config_set_value(nuevo, "PUNTERO_DIRECTO", string_itoa(buscar_bloque_libre()));
        config_set_value(nuevo, "PUNTERO_INDIRECTO", string_itoa(buscar_bloque_libre()));

        config_save(nuevo);

        dictionary_put(DICCIONARIO_FCB, nombre_archivo, nuevo);
    }
}

void actualizar_fcb(char *nombre_archivo, char *clave, char *valor)
{
    t_config *fcb = dictionary_get(DICCIONARIO_FCB, nombre_archivo);
    config_set_value(fcb, clave, valor);
    config_save(fcb);
}