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
}