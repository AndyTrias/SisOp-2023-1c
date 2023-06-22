#include "instrucciones_memoria.h"

t_paquete* crear_segmento(int id_segmento, int tamanio, int PID) {
    t_hueco* hueco = NULL;
    if (strcmp(CONFIG->algoritmo_asignacion, "FIRST") == 0) {
        hueco = get_hueco_con_first_fit(tamanio);
    } else if (strcmp(CONFIG->algoritmo_asignacion, "BEST") == 0) {
        hueco = get_hueco_con_best_fit(tamanio);
    } else if (strcmp(CONFIG->algoritmo_asignacion, "WORST") == 0) {
        hueco = get_hueco_con_worst_fit(tamanio);
    } else if (comprobar_compactacion(tamanio)){
        return crear_paquete(COMPACTAR);
        log_info(LOGGER_MEMORIA, "Se solicita compactacion");
    } else {
        log_error(LOGGER_MEMORIA, "Algoritmo de asignacion no valido");
        return NULL;
    }

    if (!hueco) {
        log_error(LOGGER_MEMORIA, "No hay hueco disponible para crear el segmento");
        return crear_paquete(OUT_OF_MEMORY);
    }

    modificar_lista_huecos(hueco, tamanio);

    t_paquete* paquete = crear_paquete(CREATE_SEGMENT);
    agregar_a_paquete_dato_serializado(paquete, &(hueco->base), sizeof(void*));
    log_info(LOGGER_MEMORIA, "PID: <%d> - Crear Segmento: <%d> - Base: <%p> - TAMAÑO: <%d>", PID, id_segmento, hueco->base, tamanio);
    return paquete;
}

void eliminar_segmento(t_list* tabla_segmentos, int id_segmento, int PID) {
    t_segmento* segmento = list_get(tabla_segmentos, id_segmento);
    if (PID != -1) log_info(LOGGER_MEMORIA, "PID: <%d> - Eliminar Segmento: <%d> - Base: <%p> - TAMAÑO: <%d>", PID, id_segmento, segmento->base, segmento->tamanio);

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

    segmento->base = NULL;
    segmento->tamanio = 0;
    list_replace(tabla_segmentos, id_segmento, segmento);
}

void finalizar_proceso(t_list* tabla_segmentos){
    for (int i = 0; i < list_size(tabla_segmentos); i++) {
        t_segmento* segmento = list_get(tabla_segmentos, i);
        if (segmento->base != NULL) {
            eliminar_segmento(tabla_segmentos, i, -1);
        }
    }

    list_destroy(tabla_segmentos);
}

char* leer_valor_direccion_fisica(long direccion_fisica){
    int tamanio = strlen((char*)direccion_fisica) + 1;
    char* valor = malloc(tamanio);
    memcpy(valor, (void*)direccion_fisica, tamanio);
    return valor;
}

void escribir_valor_direccion_fisica(char* valor, long direccion_fisica){
    void* direccion = (void*)direccion_fisica;
    int tamanio = strlen(valor) + 1;
    memcpy(direccion, valor, sizeof(strlen(valor)) + 1);
}

char* leer_fs(void* direccion_fisica, int tamanio){
    char* valor = malloc(tamanio);
    memcpy(valor, direccion_fisica, tamanio);
    return valor;
}

void escribir_fs(char* valor, void* direccion_fisica, int tamanio){
    memcpy(direccion_fisica, valor, tamanio);
}

void compactar(t_list* tablas_segmentos){
    
}