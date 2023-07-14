#include "instrucciones_memoria.h"

t_paquete* crear_segmento(int id_segmento, int tamanio, t_ctx* ctx) {

    t_hueco* hueco = NULL;
    if (strcmp(CONFIG->algoritmo_asignacion, "FIRST") == 0) {
        hueco = get_hueco_con_first_fit(tamanio);
    } else if (strcmp(CONFIG->algoritmo_asignacion, "BEST") == 0) {
        hueco = get_hueco_con_best_fit(tamanio);
    } else if (strcmp(CONFIG->algoritmo_asignacion, "WORST") == 0) {
        hueco = get_hueco_con_worst_fit(tamanio);
    } else {
        log_error(LOGGER_MEMORIA, "Algoritmo de asignacion no valido");
        return NULL;
    }

    if (!hueco && comprobar_compactacion(tamanio)){
        t_paquete* paquete = crear_paquete(COMPACTAR);
        agregar_a_paquete_dato_serializado(paquete, &tamanio, sizeof(int));
        return paquete;
    } else if (!hueco) {
        log_error(LOGGER_MEMORIA, "No hay hueco disponible para crear el segmento");
        return crear_paquete(OUT_OF_MEMORY);
    }

    modificar_lista_huecos(hueco, tamanio);

    t_segmento* segmento = list_get(ctx->tabla_segmentos, id_segmento);

    memcpy(&(segmento->base), &hueco->base, sizeof(void*));
    memcpy(&(segmento->tamanio), &tamanio, sizeof(int));

    list_add(ctx->tabla_segmentos, segmento);

    t_tabla_segmentos* ts = malloc(sizeof(t_tabla_segmentos));
    ts->PID = ctx->PID;
    ts->segmentos = ctx->tabla_segmentos;
    list_replace(TABLA_SEGMENTOS_GLOBAL, ctx->PID, ts);

    t_paquete* paquete = crear_paquete(CREATE_SEGMENT);
    agregar_a_paquete_dato_serializado(paquete, &(segmento->base), sizeof(segmento->base));
    log_info(LOGGER_MEMORIA, "PID: <%d> - Crear Segmento: <%d> - Base: <%p> - TAMAÑO: <%d>", ctx->PID, id_segmento, hueco->base, tamanio);
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
    
    t_tabla_segmentos* ts = malloc(sizeof(t_tabla_segmentos));
    ts->PID = PID;
    ts->segmentos = tabla_segmentos;
    list_replace(TABLA_SEGMENTOS_GLOBAL, PID, ts);
}

void finalizar_proceso(t_list* tabla_segmentos){
    for (int i = 1; i < list_size(tabla_segmentos); i++) {
        t_segmento* segmento = list_get(tabla_segmentos, i);
        if (segmento->base != NULL) {
            // quitar de la tabla de segmentos
            eliminar_segmento(tabla_segmentos, i, -1);
        }
    }

    list_destroy(tabla_segmentos);
}

char* leer_valor_direccion_fisica(long direccion_fisica){
    sleep(CONFIG->retardo_memoria / 500);
    int tamanio = strlen((char*)direccion_fisica) + 1;
    char* valor = malloc(tamanio);
    memcpy(valor, (void*)direccion_fisica, tamanio);
    return valor;
}

void escribir_valor_direccion_fisica(char* valor, long direccion_fisica){
    sleep(CONFIG->retardo_memoria / 500);
    void* direccion = (void*)direccion_fisica;
    memcpy(direccion, valor, sizeof(strlen(valor)) + 1);
}

void mostrar_hueco(t_hueco* hueco){
    log_info(LOGGER_MEMORIA, "Hueco: <%p> - TAMAÑO: <%d> - LIBRE: <%d>", hueco->base, hueco->tamanio, hueco->libre);
}

void mostrar_segmento(t_segmento* segmento){
    log_info(LOGGER_MEMORIA, "Segmento: <%p> - TAMAÑO: <%d>", segmento->base, segmento->tamanio);
}

void compactar(){
    log_info(LOGGER_MEMORIA, "Se solicita compactacion");
    int nuevo_tamanio = 0;
    void* base_del_primer_hueco = NULL;

    for (int i = 1; i < list_size(LISTA_HUECOS); i++) {
        t_hueco* hueco = list_get(LISTA_HUECOS, i);

        if (base_del_primer_hueco == hueco->base){
            hueco->tamanio = nuevo_tamanio;
            // eliminar los huecos que estan a la derecha del hueco que se esta compactando
            for (int j = i + 1; j < list_size(LISTA_HUECOS); j++) {
                list_remove(LISTA_HUECOS, j);
            }
            break;
        }

        if (hueco->libre) {
            // agregarlo al final de la lista de huecos
            list_remove(LISTA_HUECOS, i);
            list_add(LISTA_HUECOS, hueco);
            nuevo_tamanio += hueco->tamanio;
            if (!base_del_primer_hueco) base_del_primer_hueco = hueco->base;
            i--;
        }
    }

    // modificar las bases de los huecos en relacion a su tamanio (solo los ocupados)
    t_hueco* hueco = list_get(LISTA_HUECOS, 0);
    void* base_actual = hueco->base;
    int tamanio_actual = hueco->tamanio;

    for (int i = 1; i < list_size(LISTA_HUECOS); i++) {
        t_hueco* hueco = list_get(LISTA_HUECOS, i);
        if (!hueco->libre) {
            // modificar la tabla de segmentos
            // buscar el segmento en todas las tablas de segmentos
            for (int j = 0; j < list_size(TABLA_SEGMENTOS_GLOBAL); j++) {
                t_tabla_segmentos* tabla_segmentos = list_get(TABLA_SEGMENTOS_GLOBAL, j);
                for (int k = 1; k < list_size(tabla_segmentos->segmentos); k++) {
                    t_segmento* segmento = list_get(tabla_segmentos->segmentos, k);
                    if (segmento->base == hueco->base) {
                        segmento->base = base_actual + tamanio_actual;
                        list_replace(tabla_segmentos->segmentos, k, segmento);
                        log_info(LOGGER_MEMORIA, "PID: <%d> - Segmento: <%d> - Base: <%p> - TAMAÑO: <%d>", tabla_segmentos->PID, k, segmento->base, segmento->tamanio);
                        break;
                    }
                }
            }

            hueco->base = base_actual + tamanio_actual;
            base_actual = hueco->base;
            tamanio_actual = hueco->tamanio;
        } else {
            hueco->base = base_actual + tamanio_actual;
            tamanio_actual += hueco->tamanio;
        }

    }
}