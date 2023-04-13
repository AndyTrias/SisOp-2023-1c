#ifndef MENSAJES_H_
#define MENSAJES_H_
    
    #include <stdlib.h>
    #include <stdio.h>
    #include <unistd.h>
    #include <sys/socket.h>
    #include <netdb.h>
    #include <string.h>
    #include <commons/log.h>

// make necessary includes
    typedef enum
    {
        MENSAJE,
        PAQUETE
    }op_code;

    typedef struct
    {
        int size;
        void* stream;
    } t_buffer;

    typedef struct
    {
        op_code codigo_operacion;
        t_buffer* buffer;
    } t_paquete;
    
    t_paquete* crear_paquete(void);
    void crear_buffer(t_paquete* );
    void* serializar_paquete(t_paquete* , int );
    void enviar_mensaje(char* , int );
    int recibir_operacion(int );
    void recibir_mensaje(int, t_log* );
    
#endif

