#include "blocked.h"
// tiene que esperar todos los signal de los semaforos en SEMAFOROS_RECURSOS

pthread_t hacer_IO;

void wait(t_pcb *proceso)
{
    int recurso_id = get_id_recurso(proceso->contexto.motivos_desalojo->parametros[0]);
    if (INSTANCIAS_RECURSOS[recurso_id] == 0)
    { // no hay recursos disponibles para darle
        reemplazar_exec_por_nuevo();
        list_add(list_get(LISTAS_BLOCK, recurso_id), proceso);
    }
    else
    {
        // sem_wait(SEMAFOROS_RECURSOS[recurso_id]);
        INSTANCIAS_RECURSOS[recurso_id]--;
    };
    log_info(LOGGER_KERNEL,"PID: <%d> - Wait: <%s> - Instancias: <%d>",proceso->contexto.PID,RECURSOS[recurso_id],atoi(INSTANCIAS_RECURSOS[recurso_id]));
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
    log_info(LOGGER_KERNEL,"PID: <%d> - Signal: <%s> - Instancias: <%d>",proceso->contexto.PID,RECURSOS[recurso_id],atoi(INSTANCIAS_RECURSOS[recurso_id]));

}

void* instruccion_IO(t_pcb * proceso)
{  
    t_instruccion* instruccion_utilizable = list_get(proceso->contexto.instrucciones, proceso->contexto.program_counter -1);
    int tiempo = atoi(instruccion_utilizable->parametros[0]);
    log_info(LOGGER_KERNEL, "PID: %d - Ejecuta IO: %d", proceso->contexto.PID, tiempo);
    usleep(tiempo * 1000);
    agregar_a_lista_ready(proceso);
    return NULL;
}

void io(t_pcb *proceso)
{
    pthread_create(&hacer_IO, NULL, (void *) instruccion_IO, proceso);
    pthread_detach(hacer_IO);
    
    reemplazar_exec_por_nuevo();
}

