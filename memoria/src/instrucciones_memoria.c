#include "instrucciones_memoria.h"


void* crear_segmento(int id_segmento, int tamanio) {
    t_hueco* hueco = NULL;
    if (strcmp(CONFIG->algoritmo_asignacion, "FIRST") == 0) {
        hueco = get_hueco_con_first_fit(tamanio);
    } else if (strcmp(CONFIG->algoritmo_asignacion, "BEST") == 0) {
        hueco = get_hueco_con_best_fit(tamanio);
    } else if (strcmp(CONFIG->algoritmo_asignacion, "WORST") == 0) {
        hueco = get_hueco_con_worst_fit(tamanio);
    } else {
        log_error(LOGGER_MEMORIA, "Algoritmo de asignacion invalido");
        return NULL;
    }

    if (!hueco) {
        log_error(LOGGER_MEMORIA, "No hay hueco disponible para crear el segmento");
        return NULL;
    }

    modificar_lista_huecos(hueco, tamanio);

    return hueco->base;
}