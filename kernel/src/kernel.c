#include <utils/logs.h>
#include <utils/conexiones.h>
#include <utils/aux.h>

int main(int argc, char *argv[]) {

  if (argc != 2) {
    printf("Debe ingresar el archivo de configuracion\n");
    exit(1);
  }

  logger_y_config variables = inicializar_variables(argv[1], "./logs/kernel.log", "Kernel");
  
  t_log* logger = variables.logger;
  t_config* config = variables.config;

  char *puerto_escucha = config_get_string_value(config, "PUERTO_ESCUCHA");
  char *ip_cpu = config_get_string_value(config, "IP_CPU");
  char *puerto_cpu = config_get_string_value(config, "PUERTO_CPU");
  char *ip_filesystem = config_get_string_value(config, "IP_FILESYSTEM");
  char *puerto_filesystem = config_get_string_value(config, "PUERTO_FILESYSTEM");
  char *ip_memoria = config_get_string_value(config, "IP_MEMORIA");
  char *puerto_memoria = config_get_string_value(config, "PUERTO_MEMORIA");


  int conexion_cpu = inicializar_cliente(ip_cpu, puerto_cpu, logger);

  int conexion_memoria = inicializar_cliente(ip_memoria, puerto_memoria, logger);
  
  int conexion_filesystem = inicializar_cliente(ip_filesystem, puerto_filesystem, logger);


  inicializar_servidor(IP, puerto_escucha, logger);

  terminar_programa(conexion_cpu, logger, config);
  terminar_programa(conexion_filesystem, logger, config);
  terminar_programa(conexion_memoria, logger, config);

  return 0;
}