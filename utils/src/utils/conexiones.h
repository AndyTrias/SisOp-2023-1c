#ifndef CONEXIONES_H_
#define CONEXIONES_H_
    
    #include <sys/socket.h>
    #include <stdlib.h>
    #include <stdio.h>
    #include <netdb.h>
    #include <string.h>

    /**
    * @NAME: crear_conexion
    * @DESC: Crea una conexion con el servidor.
    */
    int crear_conexion(char *, char *);
    int iniciar_servidor(char *, char *);
    int esperar_cliente(int);

#endif
