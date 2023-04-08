#include <consola.h>

int main(int argc, char *argv[]) {

  if (argc != 2) {
    printf("Debe ingresar el archivo de configuracion\n");
    exit(1);
  }

  logger_y_config variables = inicializar_variables(argv[1], "./logs/consola.log", "Consola");

  t_log* logger = variables.logger;
  t_config* config = variables.config;

  char* ip = config_get_string_value(config, "IP_KERNEL");
  char* puerto = config_get_string_value(config, "PUERTO_KERNEL");

  int conexion_kernel = inicializar_cliente(ip, puerto, logger);

  terminar_programa(conexion_kernel, logger, config);

  return 0;
}
