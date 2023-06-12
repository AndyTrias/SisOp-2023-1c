#include "instrucciones_memoria.h"

void* crear_segmento(int id_segmento, int tamanio) {
    t_hueco* hueco = NULL;
    if (strcmp(CONFIG->algoritmo_asignacion, "FIRST") == 0) {
        hueco = get_hueco_con_first_fit(tamanio);
    } else if (strcmp(CONFIG->algoritmo_asignacion, "BEST") == 0) {
        hueco = get_hueco_con_best_fit(tamanio);
    } else if (strcmp(CONFIG->algoritmo_asignacion, "WORST") == 0) {
        hueco = get_hueco_con_worst_fit(tamanio);
    } else if (comprobar_compactacion(tamanio)){
        // aqui hay q pedirle a kernel que solicite compactacion
        log_info(LOGGER_MEMORIA, "Se solicita compactacion");
    } else {
        log_error(LOGGER_MEMORIA, "Algoritmo de asignacion no valido");
        return NULL;
    }

    if (!hueco) {
        log_error(LOGGER_MEMORIA, "No hay hueco disponible para crear el segmento");
        return NULL;
    }

    modificar_lista_huecos(hueco, tamanio);

    return hueco->base;
}

void imprimir_hueco(t_hueco* hueco) {
    log_info(LOGGER_MEMORIA, "Hueco: base %p, tamanio %d, libre %d", hueco->base, hueco->tamanio, hueco->libre);
}

void eliminar_segmento(t_list* tabla_segmentos, int id_segmento) {
    t_segmento* segmento = list_get(tabla_segmentos, id_segmento);

    //buscar hueco que tenga la misma base que el segmento y marcarlo como libre
    int index_hueco = 0;
    for (int i = 0; i < list_size(LISTA_HUECOS); i++) {
        t_hueco* hueco = list_get(LISTA_HUECOS, i);
        if (hueco->base == segmento->base) {
            hueco->libre = true;
            index_hueco = i;
            break;
        }
    }

    comprobar_consolidacion_huecos_aledanios(index_hueco);

    list_iterate(LISTA_HUECOS, (void*) imprimir_hueco);

    segmento->base = -1;
    segmento->tamanio = 0;
    list_replace(tabla_segmentos, id_segmento, segmento);
}

void comprobar_consolidacion_huecos_aledanios(int index_hueco) {
    t_hueco* hueco_actual = list_get(LISTA_HUECOS, index_hueco);
    t_hueco* hueco_anterior = NULL;
    t_hueco* hueco_siguiente = NULL;

    if (index_hueco > 0) {
        hueco_anterior = list_get(LISTA_HUECOS, index_hueco - 1);
    }

    if (index_hueco < list_size(LISTA_HUECOS) - 1) {
        hueco_siguiente = list_get(LISTA_HUECOS, index_hueco + 1);
    }

    if (hueco_anterior && hueco_anterior->libre) {
        hueco_anterior->tamanio += hueco_actual->tamanio;
        list_remove(LISTA_HUECOS, index_hueco);
        hueco_actual = hueco_anterior;
        index_hueco--;
        free(hueco_anterior);
    }

    if (hueco_siguiente && hueco_siguiente->libre) {
        hueco_actual->tamanio += hueco_siguiente->tamanio;
        list_remove(LISTA_HUECOS, index_hueco + 1);
        free(hueco_siguiente);
    }
}