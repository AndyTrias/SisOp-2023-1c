#include "manejo_fs.h"


void crear_abrir_archivo(t_pcb * proceso){
    t_instruccion* instruccion_utilizable = list_get(proceso->contexto.instrucciones, proceso->contexto.program_counter - 1);
    log_info(LOGGER_KERNEL, "PID: %d - Abrir Archivo: %s", proceso->contexto.PID, instruccion_utilizable->parametros[0]);
    
    
    FILE * pf;
    pf = fopen (instruccion_utilizable->parametros[0], "a" );
    if(pf == NULL){
        log_info(LOGGER_KERNEL, "No se pudo crear el achivo");
    }
    
    list_add(proceso->archivos_abiertos, pf);
    

    log_info(LOGGER_KERNEL, "Se creó el archivo");
    
    // cambio_de_estado(proceso->contexto.PID,"Exec","Ready");
    agregar_a_lista_ready(proceso);
    reemplazar_exec_por_nuevo();
}

void cerrar_archivo(t_pcb * proceso){
    t_instruccion* instruccion_utilizable = list_get(proceso->contexto.instrucciones, proceso->contexto.program_counter - 1);
    log_info(LOGGER_KERNEL, "PID: %d - Cerrar Archivo: %s", proceso->contexto.PID, instruccion_utilizable->parametros[0]);
    
    
    fclose(list_remove(proceso->archivos_abiertos, 0));
    
    
    agregar_a_lista_ready(proceso);
    // cambio_de_estado(proceso->contexto.PID,"Exec","Ready");
    reemplazar_exec_por_nuevo();

}