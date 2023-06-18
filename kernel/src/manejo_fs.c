#include "manejo_fs.h"
//tabla de archivos tiene que tener toda la info necesaria y una lista de procesos bloqueados

int busqueda_tabla_global(char * nombre_archivo){
  int i = 0;
  while (i < list_size(TABLA_GLOBAL_DE_ARCHIVOS_ABIERTOS)){
    t_tabla_global * auxiliar = list_get(TABLA_GLOBAL_DE_ARCHIVOS_ABIERTOS, i);
    if (strcmp(nombre_archivo, auxiliar->identificador)==0){
      return i;
    }
    i++;
  }
  return -1;
}

int busqueda_tabla_proceso(t_pcb * proceso, char * nombre_archivo){
  int i = 0;
  while (i < list_size(proceso->archivos_abiertos)){
    t_file * auxiliar = list_get(proceso->archivos_abiertos, i);
    if (strcmp(nombre_archivo, auxiliar->nombre)==0){
      return i;
    }
    i++;
  }
  return -1;
}

void agregar_entrada_tabla(char nombre, int pid){
  t_tabla_global * auxiliar = malloc(sizeof(t_tabla_global));
  strcpy(auxiliar->identificador, nombre);
  auxiliar->lista_de_procesos_bloqueados = list_create();
  auxiliar->PID = pid;
  list_add(TABLA_GLOBAL_DE_ARCHIVOS_ABIERTOS, auxiliar);
}

void eliminar_entrada_tabla(int posicion){
    t_tabla_global * auxiliar = list_get(TABLA_GLOBAL_DE_ARCHIVOS_ABIERTOS, posicion);
    list_destroy(auxiliar->lista_de_procesos_bloqueados);
    free(auxiliar);
    list_remove(TABLA_GLOBAL_DE_ARCHIVOS_ABIERTOS, posicion);
}

int existe_archivo(char nombre[30]){
  //mandar_a_fs
  //esperar respuesta
}



int f_open(t_pcb *proceso, char nombre_archivo[30]){
  int busqueda = busqueda_tabla_global(nombre_archivo);
  //t_file * archivo = malloc(sizeof(t_file));
  //strcpy(archivo->nombre, nombre_archivo);
  //agregar a tabla del proceso con el puntero en 0
  //list_add(proceso->archivos_abiertos,archivo );
  if (busqueda == -1){
    existe_archivo(nombre_archivo);
    sem_wait(&RESPUESTA_FS);
    agregar_entrada_tabla(nombre_archivo, proceso->contexto.PID);
    log_info(LOGGER_KERNEL, "PID: %d - Abrir Archivo: %s", proceso->contexto.PID, nombre_archivo);
    return 0; //puede seguir
  } else {
    //si esta en la tabla
    t_tabla_global* entrada_tabla = list_get(TABLA_GLOBAL_DE_ARCHIVOS_ABIERTOS, busqueda);
    list_add(entrada_tabla->lista_de_procesos_bloqueados, proceso);
    log_info(LOGGER_KERNEL, "PID: %d - Bloqueado por: %s", proceso->contexto.PID, nombre_archivo);
    return 1; //bloqueado
  }
}

void f_close(t_pcb *proceso, char nombre_archivo[30]){
  int busqueda = busqueda_tabla_global(nombre_archivo);
  t_tabla_global* entrada_tabla = list_get(TABLA_GLOBAL_DE_ARCHIVOS_ABIERTOS, busqueda);
  
  if (busqueda == -1 || entrada_tabla->PID != proceso->contexto.PID){
    log_error(LOGGER_KERNEL, "PID: %d - Error al cerrar Archivo: %s", proceso->contexto.PID, nombre_archivo);
  } else {
    if (list_size( entrada_tabla ->lista_de_procesos_bloqueados) == 0){
      //no hay nadie mas
      eliminar_entrada_tabla(busqueda);
    } else {
      //hay alguien mas
      list_remove(proceso->archivos_abiertos, busqueda_tabla_proceso(proceso, nombre_archivo));

      t_pcb *proceso_a_desbloquear = list_remove(entrada_tabla->lista_de_procesos_bloqueados, 0);
      //cambiar pid, cede el control sobre el archivo a otro proceso y lo desbloquea
      entrada_tabla->PID = proceso_a_desbloquear->contexto.PID;
      agregar_a_lista_ready(proceso_a_desbloquear);
      
      log_info(LOGGER_KERNEL, "PID: %d - Cerrar Archivo: %s", proceso->contexto.PID, nombre_archivo);
      
    }
  }
}

void f_seek(t_pcb *proceso, char nombre_archivo[30], int posicion){
  int busqueda = busqueda_tabla_global(nombre_archivo);
  t_tabla_global* entrada_tabla = list_get(TABLA_GLOBAL_DE_ARCHIVOS_ABIERTOS, busqueda);
  
  if (busqueda == -1 || entrada_tabla->PID != proceso->contexto.PID){
    log_error(LOGGER_KERNEL, "PID: %d - Error al hacer seek en Archivo: %s", proceso->contexto.PID, nombre_archivo);
  } else {
    //cambiar puntero
    t_file * archivo = list_get(proceso->archivos_abiertos, busqueda_tabla_proceso(proceso, nombre_archivo));
    archivo->puntero = posicion;
    log_info(LOGGER_KERNEL, "PID: %d - Actualizar puntero Archivo: %s - Puntero %d", proceso->contexto.PID, nombre_archivo, posicion);
  }
}
//Truncate, READ y WRITE

void f_truncate(t_pcb *proceso, char nombre_archivo[30]){
  int busqueda = busqueda_tabla_global(nombre_archivo);
  t_tabla_global* entrada_tabla = list_get(TABLA_GLOBAL_DE_ARCHIVOS_ABIERTOS, busqueda);
  
  if (busqueda == -1 || entrada_tabla->PID != proceso->contexto.PID){
    log_error(LOGGER_KERNEL, "PID: %d - Error al hacer truncate en Archivo: %s", proceso->contexto.PID, nombre_archivo);
  } else {
    int tamanio = proceso->contexto.motivos_desalojo->parametros[1];
   //mandar a file system archivo y tamanio
   list_add(BLOQUEADOS_FS, proceso);
   log_info(LOGGER_KERNEL, "PID: %d - Truncar Archivo: %s - Tamaño: %d", proceso->contexto.PID, nombre_archivo, tamanio);
   log_info(LOGGER_KERNEL, "PID: %d - Bloqueado por: %s", proceso->contexto.PID, nombre_archivo);



  }
}

int obtener_puntero(t_pcb *proceso, char nombre_archivo[30]){
  int busqueda = busqueda_tabla_global(nombre_archivo);
  t_tabla_global* entrada_tabla = list_get(TABLA_GLOBAL_DE_ARCHIVOS_ABIERTOS, busqueda);
  
  if (busqueda == -1 || entrada_tabla->PID != proceso->contexto.PID){
    log_error(LOGGER_KERNEL, "PID: %d - Error al hacer read en Archivo: %s", proceso->contexto.PID, nombre_archivo);
  } else {
    t_file *aux = list_get(proceso->archivos_abiertos, busqueda_tabla_proceso(proceso, nombre_archivo));
    return aux->puntero;
  }
}

void f_read(t_pcb *proceso, char nombre_archivo[30], int cant_bytes){
  int busqueda = busqueda_tabla_global(nombre_archivo);
  t_tabla_global* entrada_tabla = list_get(TABLA_GLOBAL_DE_ARCHIVOS_ABIERTOS, busqueda);
  
  if (busqueda == -1 || entrada_tabla->PID != proceso->contexto.PID){
    log_error(LOGGER_KERNEL, "PID: %d - Error al hacer read en Archivo: %s", proceso->contexto.PID, nombre_archivo);
  } else {
    int dir_fisica = proceso->contexto.motivos_desalojo->parametros[1];
    int puntero = obtener_puntero(proceso, nombre_archivo);
   //mandar a file system archivo y cant_bytes
   list_add(BLOQUEADOS_FS, proceso);
   //Leer Archivo: “PID: <PID> - Leer Archivo: <NOMBRE ARCHIVO> - Puntero <PUNTERO> - Dirección Memoria <DIRECCIÓN MEMORIA> - Tamaño <TAMAÑO>”
   //Motivo de Bloqueo: “PID: <PID> - Bloqueado por: <IO / NOMBRE_RECURSO / NOMBRE_ARCHIVO>”
   log_info(LOGGER_KERNEL, "PID: %d - Leer Archivo: %s - Puntero %d - Dirección Memoria %d - Tamaño %d", proceso->contexto.PID, nombre_archivo, puntero, dir_fisica, cant_bytes);
   log_info(LOGGER_KERNEL, "PID: %d - Bloqueado por: %s", proceso->contexto.PID, nombre_archivo);  
  }

}

void f_write(t_pcb *proceso, char nombre_archivo[30], int cant_bytes){
  int busqueda = busqueda_tabla_global(nombre_archivo);
  t_tabla_global* entrada_tabla = list_get(TABLA_GLOBAL_DE_ARCHIVOS_ABIERTOS, busqueda);
  
  if (busqueda == -1 || entrada_tabla->PID != proceso->contexto.PID){
    log_error(LOGGER_KERNEL, "PID: %d - Error al hacer read en Archivo: %s", proceso->contexto.PID, nombre_archivo);
  } else {
   //mandar a file system archivo y cant_bytes
   int dir_fisica = proceso->contexto.motivos_desalojo->parametros[1];
   int puntero = obtener_puntero(proceso, nombre_archivo);
   list_add(BLOQUEADOS_FS, proceso);
   //Escribir Archivo: “PID: <PID> - Escribir Archivo: <NOMBRE ARCHIVO> - Puntero <PUNTERO> - Dirección Memoria <DIRECCIÓN MEMORIA> - Tamaño <TAMAÑO>”
    //Motivo de Bloqueo: “PID: <PID> - Bloqueado por: <IO / NOMBRE_RECURSO / NOMBRE_ARCHIVO>”
    log_info(LOGGER_KERNEL, "PID: %d - Escribir Archivo: %s - Puntero %d - Dirección Memoria %d - Tamaño %d", proceso->contexto.PID, nombre_archivo, puntero, dir_fisica, cant_bytes);
    log_info(LOGGER_KERNEL, "PID: %d - Bloqueado por: %s", proceso->contexto.PID, nombre_archivo);

  }
}



void desbloquear_de_fs(char* nombre_archivo){
  t_tabla_global * entrada_tabla = list_get(TABLA_GLOBAL_DE_ARCHIVOS_ABIERTOS, busqueda_tabla_global(nombre_archivo));
  t_pcb *proceso = buscar_bloqueados_fs(entrada_tabla->PID);
  agregar_a_lista_ready(proceso);

}

 t_pcb * buscar_bloqueados_fs(int pid){
  int i = 0;
  while (i < list_size(BLOQUEADOS_FS)){
    t_pcb * auxiliar = list_get(BLOQUEADOS_FS, i);
    if (auxiliar->contexto.PID == pid){
      return auxiliar;
    }
    i++;
  }
  return NULL;
 }