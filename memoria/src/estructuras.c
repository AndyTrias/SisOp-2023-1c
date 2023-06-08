#include "estructuras.h"

void iniciar_estructuras(t_config* config){
    int tam_memoria = config_get_int_value(config, "TAM_MEMORIA");
    int tam_segmento_0 = config_get_int_value(config, "TAM_SEGMENTO_0");
    int cant_segmentos = config_get_int_value(config, "CANT_SEGMENTOS");
    int retardo_memoria = config_get_int_value(config, "RETARDO_MEMORIA");
    int retardo_compactacion = config_get_int_value(config, "RETARDO_COMPACTACION");
    char* algoritmo_asignacion = config_get_string_value(config, "ALGORITMO_ASIGNACION");

    MEMORIA_PRINCIPAL = malloc(tam_memoria);
    log_info(LOGGER_MEMORIA, "Se creo el espacio contiguo de memoria");

    CONFIG = malloc(sizeof(t_configuracion));
    CONFIG->tam_memoria = tam_memoria;
    CONFIG->tam_segmento_0 = tam_segmento_0;
    CONFIG->cant_segmentos = cant_segmentos;
    CONFIG->retardo_memoria = retardo_memoria;
    CONFIG->retardo_compactacion = retardo_compactacion;
    CONFIG->algoritmo_asignacion = algoritmo_asignacion;

    LISTA_HUECOS = list_create();
    t_hueco* hueco = malloc(sizeof(t_hueco));
    hueco->id_hueco = 0;
    hueco->base = tam_segmento_0;
    hueco->tamanio = tam_memoria - tam_segmento_0;
    list_add(LISTA_HUECOS, hueco);
    log_info(LOGGER_MEMORIA, "Se creo el hueco inicial de tamanio %d", hueco->tamanio);

    
}

t_list* crear_tabla_segmentos(){
    t_list* tabla_segmentos = list_create();
    for (int i = 0; i < CONFIG->cant_segmentos; i++) {
        t_segmento* segmento = malloc(sizeof(t_segmento));
        segmento->id_segmento = i;
        segmento->base = -1;
        segmento->tamanio = 0;
        list_add(tabla_segmentos, segmento);
    }
    agregar_segmento_0(tabla_segmentos);
    return tabla_segmentos;
}

void agregar_segmento_0(t_list* tabla_segmentos){
    t_segmento* segmento_0 = list_get(tabla_segmentos, 0);
    segmento_0->base = 0;
    segmento_0->tamanio = CONFIG->tam_segmento_0;
}



