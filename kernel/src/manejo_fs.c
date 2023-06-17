#include "manejo_fs.h"

typedef struct{
  bool control_de_acceso[10];
  int ubicacion_en_disco;
}t_fcb;
typedef struct {
  char identificador[30];
  int PID; //para verificar que solo el archivo abierto lo pueda operar
  t_list * lista_de_procesos_bloqueados;
}t_tabla_global;


typedef struct{
  t_fcb * puntero;
  char * modo;
}t_file;




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


int f_open(t_pcb *proceso, char nombre_archivo[30]){
  int busqueda = busqueda_tabla_global(nombre_archivo);
  if (busqueda == -1){
    // no esta
    if (/*preguntar a fs si existe*/){
      //pedirle a fs que cree el archivo
    }

    agregar_entrada_tabla(nombre_archivo, proceso->contexto.PID);
    return 1; //bloqueado
  } else {
    //esta
    list_add(list_get(TABLA_GLOBAL_DE_ARCHIVOS_ABIERTOS, busqueda) ->lista_de_procesos_bloqueados, proceso);
    //agregar a lista del proceso
    return 0; //puede seguir

  }

}

void f_close(t_pcb *proceso, char nombre_archivo[30]){
  int busqueda = busqueda_tabla_global(nombre_archivo);
  if (busqueda == -1 || list_get(TABLA_GLOBAL_DE_ARCHIVOS_ABIERTOS, busqueda) ->PID != proceso->contexto.PID){
    // no esta
    //error
  } else {
    if (list_size(list_get(TABLA_GLOBAL_DE_ARCHIVOS_ABIERTOS, busqueda) ->lista_de_procesos_bloqueados) == 0){
      //no hay nadie mas
      eliminar_entrada_tabla(busqueda);
    } else {
      //hay alguien mas
      t_pcb *proceso_a_desbloquear = list_remove(list_get(TABLA_GLOBAL_DE_ARCHIVOS_ABIERTOS, busqueda) ->lista_de_procesos_bloqueados, 0);
      //cambiar pid
      list_get(TABLA_GLOBAL_DE_ARCHIVOS_ABIERTOS, busqueda) ->PID = proceso_a_desbloquear->contexto.PID;
      agregar_a_lista_ready(proceso_a_desbloquear);
    }
  }

}



void * devuelve_tabla_global_del_archivo(t_list * tabla_global,char * nombre_archivo){
  bool recursividad(void * elemento_tabla_global){
    t_tabla_global * auxiliar = elemento_tabla_global;
    bool prueba;
    if (strcmp(nombre_archivo, auxiliar->identificador)==0){
      prueba = true;
    }
    else{
      prueba = false;
    }
    return prueba;
  }
  bool (*condicion)(void*) = &recursividad;
  return list_find(tabla_global, condicion); 
}


void crear_abrir_archivo(t_pcb * proceso){
  t_instruccion* instruccion_utilizable = list_get(proceso->contexto.instrucciones, proceso->contexto.program_counter - 1);
  
  //strcpy(archivo->path, instruccion_utilizable->parametros[0]);
  log_info(LOGGER_KERNEL, "PID: %d - Abrir Archivo: %s", proceso->contexto.PID, instruccion_utilizable->parametros[0]);
  t_file * archivo= malloc(sizeof(t_file));
  
  t_tabla_global * identidad;
  if ((identidad = devuelve_tabla_global_del_archivo(TABLA_GLOBAL_DE_ARCHIVOS_ABIERTOS, instruccion_utilizable->parametros[0])) != NULL){
    archivo->puntero = identidad->metadatos; //No sé que habrá en fcb, capáz tenga que hacer cambios en el control_de_acceso
    archivo->modo = "r"; //Aquí es donde habría que proponer un lock pero no tengo idea de qué modos va a necesitar..... 
    identidad->contador++;
    list_add(proceso->archivos_abiertos, archivo);
  }
  else{
    /*
    envio_paquete_a_file_system(instruccion_utilizable->parametros[0]);
    t_fcb * = recibo_paquete_de_file_system(); // o capaz se trata de la tabla global no tengo idea.
    strcpy(identidad->identificador, instruccion_utilizable->parametros[0]);
    identidad->metadatos->t_fcb *  ;
    archivo->puntero = identidad->metadatos; //No sé que habrá en fcb, capáz tenga que hacer cambios en el control_de_acceso
    archivo->modo = "r+"; //Control total porque es el único que está usando este archivo y r+ porque capaz el archivo estaba ya escrito; 
    identidad->contador = 1;
    lista_add(TABLA_GLOBAL_DE_ARCHIVOS_ABIERTOS, identidad);
    list_add(proceso->archivos_abiertos, archivo);
    */
  }
  
  // cambio_de_estado(proceso->contexto.PID,"Exec","Ready");
  agregar_a_lista_ready(proceso);
  reemplazar_exec_por_nuevo();
}

void cerrar_archivo(t_pcb* proceso){
    t_instruccion* instruccion_utilizable = list_get(proceso->contexto.instrucciones, proceso->contexto.program_counter - 1);
    log_info(LOGGER_KERNEL, "PID: %d - Cerrar Archivo: %s", proceso->contexto.PID, instruccion_utilizable->parametros[0]);
    t_tabla_global* identidad;
    identidad = devuelve_tabla_global_del_archivo(TABLA_GLOBAL_DE_ARCHIVOS_ABIERTOS, instruccion_utilizable->parametros[0]);
    //Falta método para encontrar la lista que quiero de momento esto funcionaría además de que habría que eliminar sus permisos.
    list_remove(proceso->archivos_abiertos, 0);
    identidad->contador--;
    if(identidad->contador == 0){
      list_remove_element(TABLA_GLOBAL_DE_ARCHIVOS_ABIERTOS, identidad);
    }  

    agregar_a_lista_ready(proceso);
    // cambio_de_estado(proceso->contexto.PID,"Exec","Ready");
    reemplazar_exec_por_nuevo();
}

/*
void seek(t_pcb * proceso){
    t_instruccion* instruccion_utilizable = list_get(proceso->contexto.instrucciones, proceso->contexto.program_counter - 1);
    char nuevo_path[30];
    strcpy(nuevo_path, instruccion_utilizable->parametros[0]);
    log_info(LOGGER_KERNEL, "PID: %d - Cerrar Archivo: %s", proceso->contexto.PID, instruccion_utilizable->parametros[0]);
    
    (proceso->archivos_abiertos, )
    
    fclose(list_remove(proceso->archivos_abiertos, 0));
    
    
    agregar_a_lista_ready(proceso);
    // cambio_de_estado(proceso->contexto.PID,"Exec","Ready");
    reemplazar_exec_por_nuevo();

}
*/