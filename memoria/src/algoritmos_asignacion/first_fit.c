#include "first_fit.h"

void crear_segmento_con_first_fit(int id_segmento, int tamanio, t_list* lista_segmentos) {
    t_hueco* hueco = NULL;
    for (int i = 0; i < list_size(LISTA_HUECOS); i++) {
        t_hueco* aux = list_get(LISTA_HUECOS, i);
        if (aux->tamanio >= tamanio) {
            hueco = aux;
            break;
        }
    }

    if (!hueco) {
        log_error(LOGGER_MEMORIA, "No hay espacio suficiente para crear el segmento");
        return;
    }

    t_segmento* segmento = malloc(sizeof(t_segmento));
    segmento->id_segmento = id_segmento;
    segmento->base = hueco->base;
    segmento->tamanio = tamanio;
    list_add(lista_segmentos, segmento);

    hueco->base += tamanio;
    hueco->tamanio -= tamanio;
    if (hueco->tamanio == 0) {
        list_remove(LISTA_HUECOS, list_get(LISTA_HUECOS, 0));
    }

    log_info(LOGGER_MEMORIA, "Se creo el segmento %d", id_segmento);
}