#include "ready.h"

void admitir_proceso()
{
    t_pcb * proceso_en_new = sacar_de_lista_new(0);
    agregar_a_lista_ready(proceso_en_new);

    enviar_mensaje("Dame la tabla de segmentos inicial", SOCKET_MEMORIA);
    // guardar la tabla de segmentos en el pcb

    // Lo hizo copilot
    // t_paquete* paquete = recibir_paquete(SOCKET_MEMORIA);
    // nuevo->contexto.segmentos = deserializar_tabla_segmentos(paquete->data);
    // free(paquete->data);
    // free(paquete);
}

t_pcb *get_proceso_por_fifo()
{
    return sacar_de_lista_ready(0);
}

t_pcb *get_proceso_por_hrrn()
{
    t_pcb *proceso = get_de_lista_ready(0);
    float tiempo_actual = temporal_gettime(TIEMPO_CORRIENDO);
    float RR = 1 + ((tiempo_actual - proceso->tiempo_llegada_ready) / proceso->estimado_prox_rafaga);
    // tiempo_actual-proceso->tiempo_llegada_ready me da el tiempo de espera en ready
    // si pasa a ready en t=3 y replanifio en t=5 entonces tiempo espera es 5(tiempo_cuatal)- 3(tiempo_llegada_ready)
    int i = 0;
    float RR_aux;
    t_pcb *proceso_aux;

    while (i < list_size(LISTA_READY))
    {
        proceso_aux = get_de_lista_ready(i);
        RR_aux = 1 + ((tiempo_actual - proceso_aux->tiempo_llegada_ready) / proceso_aux->estimado_prox_rafaga);
        if (RR_aux > RR)
        {
            proceso = proceso_aux;
        }
        i++;
    }
    sacar_elemento_de_lista_ready(proceso);
    return proceso;
}

t_pcb *ceder_proceso_a_exec()
{

    if (strcmp(ALGORITMO_PLANIFICACION, "FIFO") == 0)
    {
        return get_proceso_por_fifo();
    }
    else if (strcmp(ALGORITMO_PLANIFICACION, "HRRN") == 0)
    {
        return get_proceso_por_hrrn();
    }
    else
    {
        log_error(LOGGER_KERNEL, "No se reconoce el algoritmo de planificacion <%s>", ALGORITMO_PLANIFICACION);
        exit(EXIT_FAILURE);
    }
}