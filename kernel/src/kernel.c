#include <kernel.h>

int main(int argc, char *argv[]) {
int PID_count = 0;
  if (argc != 2) {
    printf("Debe ingresar el archivo de configuracion\n");
    exit(1);
  }


  t_log* logger = iniciar_logger("./logs/kernel.log", "KERNEL");
  t_config* config = iniciar_config(argv[1]);

  int conexion_cpu, conexion_memoria, conexion_filesystem;
  inicializar_conexiones(&conexion_cpu, &conexion_memoria, &conexion_filesystem, config, logger);

  t_pcb unPCB= crear_pcb(PID_count);
  printf("Primer PCB con PID: %d\n",unPCB.PID);
  
  unPCB= crear_pcb(PID_count);
  printf("Segundo PCB con PID: %d\n",unPCB.PID);

  enviar_mensaje("Hola fileSystem, soy el kernel",conexion_filesystem);
  enviar_mensaje("Hola CPU, soy el kernel",conexion_cpu);
  enviar_mensaje("Hola Memoria, soy el kernel",conexion_memoria);

  char *puerto_escucha = config_get_string_value(config, "PUERTO_ESCUCHA");
  inicializar_servidor(IP, puerto_escucha, logger);

  terminar_conexiones(3, conexion_cpu, conexion_memoria, conexion_filesystem);
  terminar_programa(logger, config);

  return 0;
}
t_pcb crear_pcb(int PID_count){ //debe recibir como parametro las instrucciones
  t_pcb nuevo;
  
  nuevo.PID= PID_count;
  //nuevo.instrucciones = instrucciones;
  nuevo.tiempo_desde_ult_ready= temporal_create();
  nuevo.archivos_abiertos = list_create();

  PID_count ++; // ver como hacer para que esto funcione, es porque no lo paso por referencia
  //si lo paso por referencia no puedo obtener el valor del int o al menos no se como hacerlo

  return nuevo;

}
void incrementar(int *dato,int dato_incrementado){
  *dato = dato_incrementado;
}