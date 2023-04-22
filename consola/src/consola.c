#include <consola.h>

int main(int argc, char *argv[]) {

  t_log* logger = iniciar_logger("./logs/consola.log", "CONSOLA");
  
  // Verifico que se hayan ingresado los archivos de configuracion y de instrucciones
  if (argc < 3) {
    
    if (argc < 2)
      log_error(logger, "Debe ingresar el archivo de configuracion\n");
    else
      log_error(logger, "Debe ingresar el archivo con las instrucciones\n");
    
    return EXIT_FAILURE;
  }

  
  t_config* config = iniciar_config(argv[1]);
  
  int conexion_kernel;
  inicializar_conexiones(&conexion_kernel, config, logger);

  
  // Esto iria en otro archivo + modularizado
  char buffer[100];
  FILE *f = fopen(argv[2], "r");
  if (f == NULL) {
    log_error(logger, "No se pudo abrir el archivo de instrucciones");
    return EXIT_FAILURE;
  }

  t_paquete *paquete = crear_paquete();
  while ((fgets(buffer, 100, f)) != NULL){
    
    agregar_a_paquete(paquete, buffer, strlen(buffer) + 1);
    log_info(logger, "Instruccion: %s", buffer);
  }
  
  enviar_paquete(paquete, conexion_kernel);
  
  

  terminar_conexiones(1, conexion_kernel);
  terminar_programa(logger, config);

  return 0;
}
