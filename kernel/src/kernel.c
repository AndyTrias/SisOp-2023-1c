#include <kernel.h>

int main(int argc, char *argv[]) {


  if (argc != 2) {
    printf("Debe ingresar el archivo de configuracion\n");
    exit(1);
  }

  t_config* config = iniciar_config(argv[1]);
  inicializar_variables_globales(config);

  //Hilos que maneja el kernel
  pthread_t hilo_recibir_consola,hilo2,hilo3;

  int conexion_cpu, conexion_memoria, conexion_filesystem;
  inicializar_conexiones(&conexion_cpu, &conexion_memoria, &conexion_filesystem, config);

  char *puerto_escucha = config_get_string_value(config, "PUERTO_ESCUCHA");
  int socket_servidor = crear_servidor(IP, puerto_escucha);
  //conectar_consola(socket_servidor);
  
  //hilo para recibir consolas
  pthread_create(&hilo_recibir_consola, NULL, recibir_consolas, socket_servidor);
  pthread_join(hilo_recibir_consola, NULL);

  terminar_conexiones(3, conexion_cpu, conexion_memoria, conexion_filesystem);
  terminar_programa(LOGGER_KERNEL, config);

  return 0;
}
