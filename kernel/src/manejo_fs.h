#ifndef MANEJO_FS_H
#define MANEJO_FS_H

    #include <utils/planificacion/pcb.h>
    #include <commons/collections/list.h>
    #include <utils/mensajes.h>
    #include "globales.h"
    typedef struct {
        char identificador[30];
        int PID; //para verificar que solo el archivo abierto lo pueda operar
        t_list * lista_de_procesos_bloqueados;
    }t_tabla_global;

    typedef struct{
        int puntero;
        char nombre[30];
    }t_file;

    void crear_abrir_archivo(t_pcb * proceso);
    void cerrar_archivo(t_pcb * proceso);
    void seek(t_pcb * proceso);

    
#endif