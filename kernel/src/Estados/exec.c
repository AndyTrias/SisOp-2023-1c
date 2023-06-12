#include "exec.h"

void reemplazar_proceso(t_pcb *nuevo_pcb){
    EJECUTANDO = nuevo_pcb;
}

void reemplazar_ctx(t_ctx *nuevo_ctx){
    EJECUTANDO->contexto = *nuevo_ctx;
}



void enviar_a_cpu(){
    
    t_paquete *paquete = crear_paquete(CONTEXTO);
    
    serializar_contexto(&EJECUTANDO->contexto, paquete);

    enviar_paquete(paquete, SOCKET_CPU);

    free(paquete);

    log_info(LOGGER_KERNEL,"Se envia el proceso PID: <%d> al CPU",EJECUTANDO->contexto.PID);

    if (strcmp(ALGORITMO_PLANIFICACION, "HRRN") == 0){TIEMPO_EN_CPU = temporal_create();}
}



void empezar_ciclo(){  
    EJECUTANDO = ceder_proceso_a_exec();
    enviar_a_cpu();
}

void definir_accion(int cod_op, t_pcb *proceso){
    switch (cod_op)
    {
    case YIELD:
        agregar_a_lista_ready(proceso);
        log_info(LOGGER_KERNEL, "Yield PID: <%d>",proceso->contexto.PID);
        cambio_de_estado(proceso->contexto.PID,"Exec","Ready");
        reemplazar_exec_por_nuevo();
        break;
    case EXIT:
        cambio_de_estado(proceso->contexto.PID,"Exec","Exit");
        log_info(LOGGER_KERNEL, "Finaliza el proceso <%d> - Motivo: <SUCCESS>",proceso->contexto.PID);
        terminar_proceso(proceso);
        break;
    case WAIT:
        wait(proceso);
        break;
    case SIGNAL:
        signal(proceso);
        break;
    case IO:
        io(proceso);
        break;
    case F_OPEN:
        crear_abrir_archivo(proceso);
    case F_SEEK:
        break;
    case F_CLOSE:
        cerrar_archivo(proceso);
    case CREATE_SEGMENT:
        crear_segmento(proceso);
        break;
    case DELETE_SEGMENT:
        eliminar_segmento(proceso);
        break;
    default:
        log_info(LOGGER_KERNEL, "No implementamos esta función");
        break;
    }

    
}

void reemplazar_exec_por_nuevo(){
    if (strcmp(ALGORITMO_PLANIFICACION, "HRRN") == 0){estimado_prox_rafaga();}
    t_pcb *proceso_entrante = ceder_proceso_a_exec(); //pide un proceso a ready segun el algoritmo
    reemplazar_proceso(proceso_entrante);
}

void recibir_de_cpu(int conexion_cpu){
        
    int cod_op = recibir_operacion(conexion_cpu);
    
    int size;
    void* buffer = recibir_buffer(&size, conexion_cpu);
    int* desplazamiento = malloc(sizeof(int));
    *desplazamiento = 0;
    
    t_ctx * ctx = deserializar_contexto(buffer, desplazamiento);

    reemplazar_ctx(ctx);

    log_info(LOGGER_KERNEL,"Se recibe de CPU el proceso PID: <%d>",ctx->PID);

    definir_accion(cod_op, EJECUTANDO);

    enviar_a_cpu();


}
void estimado_prox_rafaga(){
    int64_t tiempo_en_cpu= temporal_gettime(TIEMPO_EN_CPU);
    temporal_destroy(TIEMPO_EN_CPU);
    
    float proxima_rafaga= HRRN_ALFA * tiempo_en_cpu + (1-HRRN_ALFA)*EJECUTANDO->estimado_prox_rafaga;
    
    EJECUTANDO->estimado_prox_rafaga= proxima_rafaga;

    log_info(LOGGER_KERNEL,"Se realizo el estimado de proxima rafaga para el PID: <%d>, nuevo estimado: %f",EJECUTANDO->contexto.PID,EJECUTANDO->estimado_prox_rafaga);
}

void crear_segmento(t_pcb *proceso){
    // enviar a memoria CREATE_SEGMENT con sus 2 parametros (id del segmento y tamanio)

    t_paquete *paquete = crear_paquete(CREAR_SEGMENTO);
    serializar_contexto(&proceso->contexto, paquete);
    enviar_paquete(paquete, SOCKET_MEMORIA);
    free(paquete);

    int size;
    void* buffer = recibir_buffer(&size, SOCKET_MEMORIA);
    
    t_segmento* segmento = list_get(proceso->contexto.tabla_segmentos, atoi(proceso->contexto.motivos_desalojo->parametros[0]));

    memcpy(&segmento->base, buffer, sizeof(int));
    segmento->tamanio = atoi(proceso->contexto.motivos_desalojo->parametros[1]);

    log_info(LOGGER_KERNEL, "PID: <%d> - Crear Segmento - Id: <%d> - Tamaño: <%d>", proceso->contexto.PID, segmento->id_segmento, segmento->tamanio);
}

void eliminar_segmento(t_pcb *proceso){
    // enviar a memoria DELETE_SEGMENT con su parametro (id del segmento)

    t_paquete *paquete = crear_paquete(ELIMINAR_SEGMENTO);
    serializar_contexto(&proceso->contexto, paquete);
    enviar_paquete(paquete, SOCKET_MEMORIA);
    free(paquete);

    int cod_op = recibir_operacion(SOCKET_MEMORIA);

    int size;
    void* buffer = recibir_buffer(&size, SOCKET_MEMORIA);
    int desplazamiento = 0;
    t_list* tabla_segmentos_actualizada = deserializar_tabla_segmentos(buffer, &desplazamiento);
    free(buffer);

    proceso->contexto.tabla_segmentos = tabla_segmentos_actualizada;

    log_info(LOGGER_KERNEL, "PID: <%d> - Eliminar Segmento - Id Segmento: <%d>", proceso->contexto.PID, atoi(proceso->contexto.motivos_desalojo->parametros[0]));
}