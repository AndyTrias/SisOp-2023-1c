#include <consola.h>

int main(int argc, char *argv[]) {

  if (argc != 2) {
    printf("Debe ingresar el archivo de configuracion\n");
    exit(1);
  }

  
  t_log* logger = iniciar_logger("./logs/consola.log", "CONSOLA");
  t_config* config = iniciar_config(argv[1]);
  
  int conexion_kernel;
  inicializar_conexiones(&conexion_kernel, config, logger);

  enviar_mensaje("Hola Kernel, soy la consola", conexion_kernel);
  enviar_mensaje("Hola Kernel2, soy la consola2", conexion_kernel);

  terminar_conexiones(1, conexion_kernel);
  terminar_programa(logger, config);

  return 0;
}
