#ifndef PLANIFiH
#define PLANIFICADOR_H
   
    #include <globales.h>
    #include <Estados/ready.h>
    #include <Estados/exec.h>
    #include <manejo_fs.h>
    void* planificador_corto();
    void* planificador_largo();
    void* recibir_consolas(int);
    
#endif