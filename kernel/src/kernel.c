#include <kernel.h>

int main(int argc, char *argv[]) {

  if (argc != 2) {
    printf("Debe ingresar el archivo de configuracion\n");
    exit(1);
  }


  t_log* logger = iniciar_logger("kernel.log", "KERNEL");
  t_config* config = iniciar_config("config/kernel.config");

  int conexion_cpu, conexion_memoria, conexion_filesystem;
  inicializar_conexiones(&conexion_cpu, &conexion_memoria, &conexion_filesystem, config, logger);


  terminar_conexiones(3, conexion_cpu, conexion_memoria, conexion_filesystem);
  terminar_programa(logger, config);

  return 0;
}