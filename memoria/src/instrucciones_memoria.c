#include "instrucciones_memoria.h"

void crear_segmento(int id_segmento, int tamanio, t_list* tabla_segmentos) {
    if (strcmp(CONFIG->algoritmo_asignacion, "FIRST") == 0) {
        crear_segmento_con_first_fit(id_segmento, tamanio, tabla_segmentos);
    } else if (strcmp(CONFIG->algoritmo_asignacion, "BEST") == 0) {
        crear_segmento_con_best_fit(id_segmento, tamanio, tabla_segmentos);
    } else if (strcmp(CONFIG->algoritmo_asignacion, "WORST") == 0) {
        crear_segmento_con_worst_fit(id_segmento, tamanio, tabla_segmentos);
    }
}