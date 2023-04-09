#include <consola.h>

int main(int argc, char *argv[]) {

  if (argc != 2) {
    printf("Debe ingresar el archivo de configuracion\n");
    exit(1);
  }

  
  t_log* logger = iniciar_logger("./logs/consola.log", "CONSOLA");
  t_config* config = iniciar_config("./config/consola.config");
  
  int conexion_kernel;
  inicializar_conexiones(&conexion_kernel, config, logger);

  terminar_conexiones(conexion_kernel);
  terminar_programa(logger, config);

  return 0;
}
