#include <conexiones_modulos.h>

void conectar_kernel(int socket_servidor)
{
    log_info(LOGGER_FILE_SYSTEM, "Esperando kernel....");
    int socket_kernel = esperar_cliente(socket_servidor);
    log_info(LOGGER_FILE_SYSTEM, "Se conecto el kernel");
    enviado_de_kernel(&socket_kernel);
}

void enviado_de_kernel(int *socket_kernel)
{
    while (1)
    {
        int cod_op = recibir_operacion(*socket_kernel);

        if (cod_op == -1)
        {
            log_info(LOGGER_FILE_SYSTEM, "Se desconecto el kernel");
            return;
        }

        t_parametros_variables *parametros_instruccion = recibir_paquete_kernel(*socket_kernel);
        atender_solicitudes(cod_op, parametros_instruccion);
        liberar_parametros_variables(parametros_instruccion);
        free(parametros_instruccion);
    }
}

t_parametros_variables *recibir_paquete_kernel(int socket_kernel)
{
    int size;
    void *buffer = recibir_buffer(&size, socket_kernel);

    int *desplazamiento = malloc(sizeof(int));
    *desplazamiento = 0;

    t_parametros_variables *parametros = deserealizar_motivos_desalojo(buffer, desplazamiento);

    return parametros;
}

char* leer_direccion_de_memoria(char *direccion_fisica)
{
    t_parametros_variables *parametros = malloc(sizeof(t_parametros_variables));
    agregar_parametro_variable(parametros, direccion_fisica);

    t_paquete *paquete = crear_paquete(F_WRITE);
    serializar_motivos_desalojo(parametros, paquete);
    enviar_paquete(paquete, SOCKET_MEMORIA);

    liberar_parametros_variables(parametros);
    free(paquete);

    return recibir_mensaje(SOCKET_MEMORIA);

}

void escribir_valor_en_memoria(char *direccion_fisica, char *contenido)
{
    t_parametros_variables *parametros = malloc(sizeof(t_parametros_variables));
    agregar_parametro_variable(parametros, contenido);
    agregar_parametro_variable(parametros, direccion_fisica);

    t_paquete *paquete = crear_paquete(F_READ);
    serializar_motivos_desalojo(parametros, paquete);
    enviar_paquete(paquete, SOCKET_MEMORIA);

    liberar_parametros_variables(parametros);
    free(paquete);
    char *mensaje = recibir_mensaje(SOCKET_MEMORIA);

    if (strcmp(mensaje, "OK"))
        log_error(LOGGER_FILE_SYSTEM, "No se pudo escribir la direccion de memoria");
}

