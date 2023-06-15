#ifndef MANEJO_FS_H
#define MANEJO_FS_H

    #include <utils/planificacion/pcb.h>
    #include <commons/collections/list.h>
    #include <utils/mensajes.h>
    #include <globales.h>

    void crear_abrir_archivo(t_pcb * proceso);
    void cerrar_archivo(t_pcb * proceso);
    void seek(t_pcb * proceso);
#endif