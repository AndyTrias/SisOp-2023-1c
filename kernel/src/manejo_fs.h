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

    int busqueda_tabla_global(char * );
    int busqueda_tabla_proceso(t_pcb * , char * );
    int obtener_puntero(t_pcb *, char* );
    void agregar_entrada_tabla(char* , int );
    void eliminar_entrada_tabla(int );
    void existe_archivo(char* );
    void solicitar_creacion();
    void solicitar_truncamiento();
    void solicitar_lectura(int,int,int);
    void solicitar_escritura(int,int);
    int f_open(t_pcb *, char* );
    void f_close(t_pcb *, char* );
    void f_seek(t_pcb *, char* , char* );
    void f_truncate(t_pcb *, char* , char* );
    void f_read(t_pcb *, char* );
    void f_write(t_pcb *, char*);
    void desbloquear_de_fs(char* );
    t_pcb * buscar_bloqueados_fs(int );


    
#endif