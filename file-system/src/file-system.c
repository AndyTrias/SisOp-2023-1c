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
    t_config* configIp = iniciar_config("/so-deploy/tp-2023-1c-Los-pibardos-de-operativos/utils/src/utils/config/ips.config");

    inicializar_conexiones(&SOCKET_MEMORIA, config);
    
    inicializar_archivos(config);
    // Caragamos todos los fcb en la memoria
    levantar_diccionario_fcb(config);


    // Test de solicitudes sin conexion
    // t_parametros_variables *parametros_instruccion = malloc(sizeof(t_parametros_variables));
    // parametros_instruccion->cantidad_parametros = 0;

    // agregar_parametro_variable(parametros_instruccion, "archivoTestFcb");
    // atender_solicitudes(F_CREATES, parametros_instruccion);


    // agregar_parametro_variable(parametros_instruccion, "152");
    // atender_solicitudes(F_TRUNCATE, parametros_instruccion);
    // liberar_parametros_variables(parametros_instruccion);
    
    // agregar_parametro_variable(parametros_instruccion, "archivoTestFcb");
    // agregar_parametro_variable(parametros_instruccion, "0");
    // agregar_parametro_variable(parametros_instruccion, "88"); // Tamanio
    // agregar_parametro_variable(parametros_instruccion, "50"); // Puntero
    // atender_solicitudes(F_READ, parametros_instruccion);


    // liberar_parametros_variables(parametros_instruccion);    


    

    char *puerto_escucha = config_get_string_value(config, "PUERTO_ESCUCHA");
    char* ip = config_get_string_value(configIp, "IP_FILE-SYSTEM");
    int socket_servidor = crear_servidor(ip, puerto_escucha);

    conectar_kernel(socket_servidor);

    terminar_programa(LOGGER_FILE_SYSTEM, config);
    terminar_conexiones(1, SOCKET_MEMORIA);

    return 0;
}