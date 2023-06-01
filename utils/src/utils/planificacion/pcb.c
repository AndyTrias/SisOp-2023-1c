#include "pcb.h"

void agregar_parametro_desalojo(t_ctx* ctx, char* parametro){
    ctx->motivos_desalojo->parametros = realloc(ctx->motivos_desalojo->parametros, sizeof(char*) * (ctx->motivos_desalojo->cantidad_parametros + 1));
    ctx->motivos_desalojo->parametros[ctx->motivos_desalojo->cantidad_parametros] = parametro;
    ctx->motivos_desalojo->cantidad_parametros++;
};
