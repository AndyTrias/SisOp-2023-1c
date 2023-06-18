#include <file-system.h>

int main(int argc, char *argv[])
{

    LOGGER_FILE_SYSTEM = iniciar_logger("./logs/file-system.log", "FILE SYSTEM");

    if (argc != 2)
    {
        log_error(LOGGER_FILE_SYSTEM, "Debe ingresar el archivo de configuracion\n");
        return EXIT_FAILURE;
    }

    t_config *config = iniciar_config(argv[1]);

    inicializar_archivos(config);

    // Estrucutra administrativa para manejar los fcb
    // modularizar en un archivo aparte
    DICCIONARIO_FCB = dictionary_create();
    char* path_fcb = config_get_string_value(config, "PATH_FCB");
    DIR *directorio = opendir(path_fcb);
    if (directorio == NULL)
    {
        log_error(LOGGER_FILE_SYSTEM, "Error al abrir el directorio de FCB.\n");
        abort();
    }

    struct dirent *archivo;
    while ((archivo = readdir(directorio)) != NULL)
    {
        if (archivo->d_type == DT_REG)
        { 
            // Solo procesar archivos regulares (ignorar directorios y otros tipos)
            char *ruta_archivo = string_from_format("%s/%s", path_fcb, archivo->d_name);
            char* nombre_archivo = string_from_format("%s", archivo->d_name);
            
            t_config *config_archivo = iniciar_config(ruta_archivo);
            dictionary_put(DICCIONARIO_FCB, nombre_archivo, config_archivo);

            // Realizar operaciones con el archivo
            log_info(LOGGER_FILE_SYSTEM,  "Archivo encontrado: %s\n", ruta_archivo);
        }
    }
    t_config *config_deseada = dictionary_get(DICCIONARIO_FCB, "Notas1erParcialK9999");
    config_set_value(config_deseada, "TAMANIO_ARCHIVO", "100");
    config_save(config_deseada);

    closedir(directorio);


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