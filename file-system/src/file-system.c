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

    // Caragamos todos los fcb en la memoria
    levantar_diccionario_fcb(config);


    // Test de solicitudes sin conexion
    t_parametros_variables *parametros_instruccion = malloc(sizeof(t_parametros_variables));
    parametros_instruccion->cantidad_parametros = 0;
    agregar_parametro_variable(parametros_instruccion, "archivoTestFcb2");

    atender_solicitudes(F_CREATE, parametros_instruccion);
    atender_solicitudes(F_OPEN, parametros_instruccion);
    
    agregar_parametro_variable(parametros_instruccion, "10");
    atender_solicitudes(F_TRUNCATE, parametros_instruccion);

    liberar_parametros_variables(parametros_instruccion);
    agregar_parametro_variable(parametros_instruccion, "archivoTestFcb2");
    agregar_parametro_variable(parametros_instruccion, "180");
    atender_solicitudes(F_TRUNCATE, parametros_instruccion);

    liberar_parametros_variables(parametros_instruccion);    


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