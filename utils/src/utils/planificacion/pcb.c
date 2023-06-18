#include "pcb.h"

void agregar_parametro_desalojo(t_ctx* ctx, char* parametro) {
    
    agregar_parametro_variable(ctx->motivos_desalojo, parametro);
}

void agregar_parametro_variable(t_parametros_variables* parametros_variables, char* parametro){

    int cantidad_parametros = parametros_variables->cantidad_parametros;

    parametros_variables->parametros = realloc(parametros_variables->parametros, sizeof(char*) * (cantidad_parametros + 1));

    parametros_variables->parametros[cantidad_parametros] = malloc(strlen(parametro) + 1);
    strcpy(parametros_variables->parametros[cantidad_parametros], parametro);

    parametros_variables->cantidad_parametros++;
}

void liberar_parametros_desalojo(t_ctx* ctx) {
    liberar_parametros_variables(ctx->motivos_desalojo);
}


void liberar_parametros_variables(t_parametros_variables* parametros) {
    for (int i = 0; i < parametros->cantidad_parametros; i++) {
        free(parametros->parametros[i]);
    }

    parametros->cantidad_parametros = 0;
}


