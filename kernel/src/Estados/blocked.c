#include "blocked.h"
// tiene que esperar todos los signal de los semaforos en SEMAFOROS_RECURSOS


void wait(t_pcb *proceso)
{
    int recurso_id = get_id_recurso(proceso->contexto.motivos_desalojo->parametros[0]);
    if (INSTANCIAS_RECURSOS[recurso_id] == 0)
    { // no hay recursos disponibles para darle
        log_info(LOGGER_KERNEL, "Se recibio un mensaje de wait");
        reemplazar_exec_por_nuevo();
        list_add(list_get(LISTAS_BLOCK, recurso_id), proceso);
    }
    else
    {
        // sem_wait(SEMAFOROS_RECURSOS[recurso_id]);
        log_info(LOGGER_KERNEL, "Se recibio un mensaje de wait del recurso %d", recurso_id);
        INSTANCIAS_RECURSOS[recurso_id]--;
    };
}

void signal(t_pcb *proceso)
{
    int recurso_id = get_id_recurso(proceso->contexto.motivos_desalojo->parametros[0]);
    t_list* lista_del_recurso = list_get(LISTAS_BLOCK, recurso_id);
    if (list_size(lista_del_recurso) > 0)
    {
        t_pcb *proceso_a_desbloquear = list_remove(list_get(LISTAS_BLOCK, recurso_id), 0);
        agregar_a_lista_ready(proceso_a_desbloquear);
    }
    else
    {
        // sem_signal(SEMAFOROS_RECURSOS[recurso_id]);
        INSTANCIAS_RECURSOS[recurso_id]++;
    }
    log_info(LOGGER_KERNEL, "Se recibio un mensaje de signal del recurso %d", recurso_id);

    void* algo_en_execute()
{
    reemplazar_exec_por_nuevo();
    log_info(LOGGER_KERNEL, "TERMINO ESTA COSA");
    return NULL;
}


void io(t_pcb *proceso){
    t_instruccion* instruccion_utilizable = list_get(proceso->contexto.instrucciones, proceso->contexto.program_counter -1);
    int tiempo_io = atoi(instruccion_utilizable->parametros[0]); 
    log_info(LOGGER_KERNEL, "PID: %d - Ejecuta IO: %d", proceso->contexto.PID, tiempo_io);
    // NO está llegando :(. Era para probar que onda
    /*
    list_add(LISTAS_BLOCK, proceso);


    pthread_t algo_en_ready;
    list_add(LISTAS_BLOCK, proceso);

    pthread_create(&algo_en_ready, NULL, (void *) algo_en_execute, NULL);
    while (tiempo_io!=0)
    {
        sleep(10);
        tiempo_io--;
        log_info(LOGGER_KERNEL, "CICLO");
    }
    
    
    t_pcb *proceso_a_desbloquear = list_remove(LISTAS_BLOCK, 0);
    agregar_a_lista_ready(proceso_a_desbloquear);
    pthread_join(algo_en_ready, NULL);
    */  
}
}

