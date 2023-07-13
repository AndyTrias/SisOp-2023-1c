#include "manejo_memoria.h"

void crear_segmento(t_pcb *proceso){// enviar a memoria CREATE_SEGMENT con sus 2 parametros (id del segmento y tamanio)
    //se solicita la creacion del segmento
    t_paquete *paquete = crear_paquete(CREATE_SEGMENT);
    serializar_contexto(&proceso->contexto, paquete);
    enviar_paquete(paquete, SOCKET_MEMORIA);
    free(paquete);

    int cod_op = recibir_operacion(SOCKET_MEMORIA);
    int size;
    void* buffer;
    switch(cod_op){
        case CREATE_SEGMENT:
            buffer = recibir_buffer(&size, SOCKET_MEMORIA);

            t_segmento* segmento = list_get(proceso->contexto.tabla_segmentos, atoi(proceso->contexto.motivos_desalojo->parametros[0]));

            memcpy(&(segmento->base), buffer, sizeof(void*));
            segmento->tamanio = atoi(proceso->contexto.motivos_desalojo->parametros[1]);

            pthread_mutex_unlock(&SOLICITUD_MEMORIA);  
            log_info(LOGGER_KERNEL, "PID: <%d> - Crear Segmento - Id: <%d> - Tamaño: <%d>", proceso->contexto.PID, segmento->id_segmento, segmento->tamanio);
            break;
        case COMPACTAR:
            log_info(LOGGER_KERNEL,"Compactación: <Se solicitó compactación / Esperando Fin de Operaciones de FS>");
            pthread_mutex_lock(&SOLICITUD_FS);
            
            t_paquete *paquete = crear_paquete(COMPACTAR);
            enviar_paquete(paquete, SOCKET_MEMORIA);
            free(paquete);

            recibir_operacion(SOCKET_MEMORIA);
            t_list* tablas_de_segmentos_actualizadas = recibir_todas_las_tablas_segmentos(SOCKET_MEMORIA);
            actualizar_tablas_de_segmentos(tablas_de_segmentos_actualizadas);

            pthread_mutex_unlock(&SOLICITUD_FS);
            log_info(LOGGER_KERNEL, "Se finalizó el proceso de compactación");
            break;
        case OUT_OF_MEMORY:
            pthread_mutex_unlock(&SOLICITUD_MEMORIA);
            log_info(LOGGER_KERNEL,"Finaliza el proceso <%d> - Motivo: <OUT_OF_MEMORY>",proceso->contexto.PID);
            terminar_proceso(proceso);
            break;
    }
    
}

void eliminar_segmento(t_pcb *proceso){
    // enviar a memoria DELETE_SEGMENT con su parametro (id del segmento)

    t_paquete *paquete = crear_paquete(DELETE_SEGMENT);
    serializar_contexto(&proceso->contexto, paquete);
    enviar_paquete(paquete, SOCKET_MEMORIA);
    free(paquete);

    recibir_operacion(SOCKET_MEMORIA);
    t_list* tabla_segmentos_actualizada = recibir_tabla_segmentos(SOCKET_MEMORIA);

    proceso->contexto.tabla_segmentos = tabla_segmentos_actualizada;

    pthread_mutex_unlock(&SOLICITUD_MEMORIA);    

    log_info(LOGGER_KERNEL, "PID: <%d> - Eliminar Segmento - Id Segmento: <%d>", proceso->contexto.PID, atoi(proceso->contexto.motivos_desalojo->parametros[0]));
}
void actualizar_tablas_de_segmentos(t_list* lista_segmentos){
    int i = 0;
    t_tabla_segmentos* tabla_segmentos_por_proceso = list_get(lista_segmentos, i);
    t_pcb* proceso_a_actualizar;

    while(i<list_size(lista_segmentos)){
        tabla_segmentos_por_proceso = list_get(lista_segmentos, i);
        
        proceso_a_actualizar= buscar_proceso(tabla_segmentos_por_proceso->PID);
        if(proceso_a_actualizar!= NULL)proceso_a_actualizar->contexto.tabla_segmentos = tabla_segmentos_por_proceso->segmentos;
        
        i++;
    }

}
t_pcb* buscar_proceso(int pid_buscado){
    int resulatdo_buesqueda; //en -1 entoces no lo encontro
    if(EJECUTANDO->contexto.PID == pid_buscado) return EJECUTANDO;

    resulatdo_buesqueda= buscar_ready(pid_buscado);
    if(resulatdo_buesqueda!=-1){
        return get_de_lista_ready(resulatdo_buesqueda);
    }
    t_pcb* proceso = buscar_block(pid_buscado);
   if(proceso != NULL) return proceso;

   resulatdo_buesqueda = buscar_block_fs(pid_buscado);
   if(resulatdo_buesqueda!=-1) return get_de_lista_blockfs(resulatdo_buesqueda);

   log_info(LOGGER_KERNEL,"No hay proceso con PID: <%d> al que se le pueda actualizar la tabla de segmetnos", pid_buscado); 

    return NULL;
}

int buscar_ready(int pid_buscado){
    int i=0;
    t_pcb *proceso_aux;
    while(i<tamnio_lista_ready()){
        proceso_aux = get_de_lista_ready(i);
        if(proceso_aux->contexto.PID == pid_buscado) return i;
        i++;
    }
    return -1;
}
t_pcb* buscar_block(int pid_buscado){
    int i=0;
    int j=0;
    t_list* aux;
    t_pcb* proceso_aux;
    while(i<list_size(LISTAS_BLOCK)){
        aux = list_get(LISTAS_BLOCK,i);
        while(j<list_size(aux)){
            proceso_aux=list_get(aux,j);
            if(proceso_aux->contexto.PID == pid_buscado) return proceso_aux;
            j++;
        }
        j=0;
        i++;
    }
    return NULL;
}
int buscar_block_fs(int pid_buscado){
    int i = 0;
    t_pcb *proceso_aux;
    while(i<tamnio_lista_blockfs()){
        proceso_aux = get_de_lista_blockfs(i);
        if(proceso_aux->contexto.PID == pid_buscado) return i;
        i++;
    }
    return -1;
}