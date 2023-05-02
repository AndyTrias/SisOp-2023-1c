#include "ready.h"

int get_grado_de_multiprogramacion(int procesos_en_blocked)
{
    return list_size(LISTA_READY) + procesos_en_blocked + get_procesos_en_cpu(); // el 1 es el que esta en el cpu. ver como se consiguen los procesos del blocked
}

int get_procesos_en_cpu(){
    return 1;
}

bool hay_espacio_ready()
{
    return (GRADO_MAX_MULTIPROGRAMACION < get_grado_de_multiprogramacion(2)); // implementar procesos en blocked
}

void agregar_a_ready_si_hay_espacio()
{
    if (hay_espacio_ready())
    {
        admitir_proceso();

        // Controlar si hay algo ejecutandose en CPU y mandarle el nuevo proceso
        empezar_ciclo_si_vacio();
    }
}


void admitir_proceso()
{ // se usa una vez que se sabe que hay espacio para un proceso mas

    list_add(LISTA_READY, (get_proceso_desde_new(LISTA_NEW)));
}

t_pcb *get_proceso_por_fifo()
{
    return list_remove(LISTA_READY, 0);
}

t_pcb *get_proceso_por_hrrn()
{
    return list_remove(LISTA_READY, 0);
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
        return get_proceso_por_fifo(LISTA_READY);
    }
    else if (strcmp(ALGORITMO_PLANIFICACION, "HRRN") == 0)
    {
        return get_proceso_por_hrrn(LISTA_READY);
    }
    else
    {
        log_error(LOGGER_KERNEL, "No se reconoce el algoritmo de planificacion <%s>", ALGORITMO_PLANIFICACION);
        exit(EXIT_FAILURE);
    }
}
