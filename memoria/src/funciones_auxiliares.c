#include "funciones_auxiliares.h"

// AUX CREATE_SEGMENT
t_hueco* get_hueco_con_first_fit(int tamanio){
    t_hueco* hueco = NULL;
    // devuelve el primer hueco que encuentre que este libre
    for (int i = 0; i < list_size(LISTA_HUECOS); i++) {
        t_hueco* hueco_actual = list_get(LISTA_HUECOS, i);
        if (hueco_actual->libre && hueco_actual->tamanio >= tamanio) {
            hueco = hueco_actual;
            break;
        }
    }
    return hueco;
}

t_hueco* get_hueco_con_best_fit(int tamanio){
    t_hueco* hueco = NULL;
    int tamanio_hueco = CONFIG->tam_memoria;
    for (int i = 0; i < list_size(LISTA_HUECOS); i++) {
        t_hueco* hueco_actual = list_get(LISTA_HUECOS, i);
        if (hueco_actual->libre && hueco_actual->tamanio >= tamanio && hueco_actual->tamanio < tamanio_hueco) {
            hueco = hueco_actual;
            tamanio_hueco = hueco_actual->tamanio;
        }
    }
    return hueco;
}

t_hueco* get_hueco_con_worst_fit(int tamanio){
    t_hueco* hueco = NULL;
    int tamanio_hueco = 0;
    for (int i = 0; i < list_size(LISTA_HUECOS); i++) {
        t_hueco* hueco_actual = list_get(LISTA_HUECOS, i);
        if (hueco_actual->libre && hueco_actual->tamanio >= tamanio && hueco_actual->tamanio > tamanio_hueco) {
            hueco = hueco_actual;
            tamanio_hueco = hueco_actual->tamanio;
        }
    }
    return hueco;
}

bool comprobar_compactacion(int tamanio){
    // sumar el tamaño de todos los hueccos libres
    int tamanio_huecos_libres = 0;
    for (int i = 0; i < list_size(LISTA_HUECOS); i++) {
        t_hueco* hueco_actual = list_get(LISTA_HUECOS, i);
        if (hueco_actual->libre) {
            tamanio_huecos_libres += hueco_actual->tamanio;
        }
    }

    return tamanio_huecos_libres >= tamanio;
}

void modificar_lista_huecos(t_hueco* hueco, int tamanio) {
    // dividir el hueco en 2, uno para el segmento y otro para el hueco restante
    t_hueco* hueco_restante = malloc(sizeof(t_hueco));
    hueco_restante->base = hueco->base + tamanio;
    hueco_restante->tamanio = hueco->tamanio - tamanio;
    hueco_restante->libre = true;

    hueco->tamanio = tamanio;
    hueco->libre = false;

    // reemplazar el hueco por los 2 nuevos (usar las funciones del commons)
    list_remove_element(LISTA_HUECOS, hueco);
    list_add_in_index(LISTA_HUECOS, list_size(LISTA_HUECOS), hueco);
    list_add_in_index(LISTA_HUECOS, list_size(LISTA_HUECOS), hueco_restante);
}




// AUX DELETE_SEGMENT
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

