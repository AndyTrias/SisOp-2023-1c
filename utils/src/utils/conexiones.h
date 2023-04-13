#ifndef CONEXIONES_H_
#define CONEXIONES_H_
    
    #include <stdlib.h>
    #include <netdb.h>
    #include <sys/socket.h>
    #include <string.h>
    #include <unistd.h>
    #include <stdarg.h>
    #include<commons/collections/list.h>
    #include <utils/logs.h>
    #include <utils/mensajes.h>
    
    #define IP "127.0.0.1"

    void inicializar_servidor(char*, char*, t_log* );
    int inicializar_cliente(char* , char* , t_log* );
    int crear_conexion(char *, char *);
    int crear_servidor(char *, char *);
    int esperar_cliente(int);
    void terminar_conexiones(int, ...);
    

    

    

#endif
