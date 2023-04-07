#ifndef AUX_H_
#define AUX_H_
    
    #include <utils/logs.h>
    #include <utils/conexiones.h>
    #include <utils/configs.h>
    #include <stdio.h>
    #include <stdlib.h>
    #include <unistd.h>
    #define IP "127.0.0.1"
    
    typedef struct {
        t_log* logger;
        t_config* config;
    } logger_y_config;

    void terminar_programa(int, t_log*, t_config*);
    logger_y_config inicializar_variables(char*, char*, char*);
    void inicializar_servidor(char*, char*, t_log*);
    int inicializar_cliente(char*, char*, t_log*);

#endif
