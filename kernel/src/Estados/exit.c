#include "exit.h"

void mostrar_recurso(char* a){
    printf("%s", a);
}

void terminar_proceso(t_pcb *proceso){
    // Revisar si tiene un recurso asignado y eliminarlo
    if (!list_is_empty(proceso->recursos_en_uso)){
        int i;
        for (i = 0; i < list_size(proceso->recursos_en_uso); i++)
        {
            list_iterate(proceso->recursos_en_uso, (void*) mostrar_recurso);
            char* asd = list_remove(proceso->recursos_en_uso, i);
            signal(proceso,list_remove(proceso->recursos_en_uso,i));
        }
    }
    // destruir proceso
    list_destroy(proceso->contexto.instrucciones);
    list_destroy(proceso->archivos_abiertos);
    list_destroy(proceso->recursos_en_uso);

    pthread_mutex_lock(&SOLICITUD_MEMORIA);
    
    t_paquete* paquete = crear_paquete(TERMINAR);
    agregar_a_paquete_dato_serializado(paquete, &proceso->contexto.PID, sizeof(int));
    serializar_tabla_segmentos(proceso->contexto.tabla_segmentos, paquete);
    enviar_paquete(paquete, SOCKET_MEMORIA);
    free(paquete);

    recibir_operacion(SOCKET_MEMORIA);
    recibir_mensaje(SOCKET_MEMORIA);

    paquete = crear_paquete(TERMINAR);
    enviar_paquete(paquete, proceso->socket_consola);
    free(paquete);
    


    pthread_mutex_unlock(&SOLICITUD_MEMORIA);
    sem_post(&GRADO_MULTIPROGRAMACION);
    reemplazar_exec_por_nuevo();
}