#include "manejo_fs.h"

typedef struct {
  char identificador[30];
  int contador;
  t_fcb * metadatos;
}t_tabla_global;

typedef struct{
  bool control_de_acceso[10];
  int ubicacion_en_disco;
}t_fcb;

typedef struct{
  t_fcb * puntero;
  char * modo;
}t_file;


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
  
  strcpy(archivo->path, instruccion_utilizable->parametros[0]);
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

void cerrar_archivo(t_pcb * proceso){
    t_instruccion* instruccion_utilizable = list_get(proceso->contexto.instrucciones, proceso->contexto.program_counter - 1);
    log_info(LOGGER_KERNEL, "PID: %d - Cerrar Archivo: %s", proceso->contexto.PID, instruccion_utilizable->parametros[0]);
    t_tabla_global * identidad;
    identidad = devuelve_tabla_global_del_archivo(TABLA_GLOBAL_DE_ARCHIVOS_ABIERTOS, instruccion_utilizable->parametros[0];
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