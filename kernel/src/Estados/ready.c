#include "ready.h"


void admitir_proceso()
{ // se usa una vez que se sabe que hay espacio para un proceso mas
    agregar_a_lista_ready(get_proceso_desde_new());
}

t_pcb *get_proceso_por_fifo()
{
    return sacar_de_lista_ready(0);
}

t_pcb *get_proceso_por_hrrn()
{
    int ganador = 0; // proceso_con_menor_RR()
    return sacar_de_lista_ready(ganador);
}

// t_pcb *HRRN()
// {
//     t_pcb *proceso = list_get(LISTA_READY, 0);
//     int tiempo_espera = temporal_get_age(proceso->tiempo_desde_ult_ready);
//     float respuesta = (tiempo_espera + proceso->estimacion_anterior) / proceso->estimacion_anterior;
//     int i = 0;
//     int tiempo_espera_aux;
//     float respuesta_aux;
//     t_pcb *proceso_aux;
//     while (i < list_size(LISTA_READY))
//     {
//         proceso_aux = list_get(LISTA_READY, i);
//         tiempo_espera_aux = temporal_get_age(proceso_aux->tiempo_desde_ult_ready);
//         respuesta_aux = (tiempo_espera_aux + proceso_aux->estimacion_anterior) / proceso_aux->estimacion_anterior;
//         if (respuesta_aux > respuesta)
//         {
//             proceso = proceso_aux;
//             tiempo_espera = tiempo_espera_aux;
//             respuesta = respuesta_aux;
//         }
//         i++;
//     }
//     return list_remove(LISTA_READY, i);
// }

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
