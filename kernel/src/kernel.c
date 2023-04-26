#include <kernel.h>

int main(int argc, char *argv[]) {


  if (argc != 2) {
    printf("Debe ingresar el archivo de configuracion\n");
    exit(1);
  }


  logger_kernel = iniciar_logger("./logs/kernel.log", "KERNEL");
  t_config* config = iniciar_config(argv[1]);

  int conexion_cpu, conexion_memoria, conexion_filesystem;
  inicializar_conexiones(&conexion_cpu, &conexion_memoria, &conexion_filesystem, config, logger_kernel);

  t_pcb unPCB= crear_pcb();
  
  unPCB= crear_pcb();

  enviar_mensaje("Hola fileSystem, soy el kernel",conexion_filesystem);
  enviar_mensaje("Hola CPU, soy el kernel",conexion_cpu);
  enviar_mensaje("Hola Memoria, soy el kernel",conexion_memoria);

  char *puerto_escucha = config_get_string_value(config, "PUERTO_ESCUCHA");
  int socket_servidor = crear_servidor(IP, puerto_escucha);
  conectar_consola(socket_servidor);

  terminar_conexiones(3, conexion_cpu, conexion_memoria, conexion_filesystem);
  terminar_programa(logger_kernel, config);

  return 0;
}
