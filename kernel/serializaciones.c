void serializar_contexto(t_ctx *ctx, t_paquete *paquete){
    // Serialio PID y PC
    agregar_a_paquete_dato_serializado(paquete, ctx->pid, sizeof(ctx->pid));
    agregar_a_paquete_dato_serializado(paquete, &ctx->pc, sizeof(ctx->pc));

    // Serializo Instrucciones
    serializar_instrucciones(paquete, ctx->instrucciones);

    // Serializo Registros
    serializar_registros(paquete, ctx->registros);
    
}

void serializar_instrucciones(t_paquete* paquete, t_list *instrucciones){
    int cant_instrucciones = list_size(ctx->instrucciones);
    for (int i =0; i<cant_instrucciones; i++){
        t_instruccion *instruccion = list_get(ctx->instrucciones, i);
        serializar_instruccion(instruccion, paquete);
    }
}
void serializar_registros(t_paquete *paquete, t_ctx *ctx)
    {
        agregar_a_paquete_dato_serializado(paquete, &(ctx->registros.AX), sizeof(ctx->registros.AX));
        agregar_a_paquete_dato_serializado(paquete, &(ctx->registros.BX), sizeof(ctx->registros.BX));
        agregar_a_paquete_dato_serializado(paquete, &(ctx->registros.CX), sizeof(ctx->registros.CX));
        agregar_a_paquete_dato_serializado(paquete, &(ctx->registros.DX), sizeof(ctx->registros.DX));
        agregar_a_paquete_dato_serializado(paquete, &(ctx->registros.EAX), sizeof(ctx->registros.EAX));
        agregar_a_paquete_dato_serializado(paquete, &(ctx->registros.EBX), sizeof(ctx->registros.EBX));
        agregar_a_paquete_dato_serializado(paquete, &(ctx->registros.ECX), sizeof(ctx->registros.ECX));
        agregar_a_paquete_dato_serializado(paquete, &(ctx->registros.EDX), sizeof(ctx->registros.EDX));
        agregar_a_paquete_dato_serializado(paquete, &(ctx->registros.RAX), sizeof(ctx->registros.RAX));
        agregar_a_paquete_dato_serializado(paquete, &(ctx->registros.RBX), sizeof(ctx->registros.RBX));
        agregar_a_paquete_dato_serializado(paquete, &(ctx->registros.RCX), sizeof(ctx->registros.RCX));
        agregar_a_paquete_dato_serializado(paquete, &(ctx->registros.RDX), sizeof(ctx->registros.RDX));
    }