#ifndef FCB_H
#define FCB_H
    
    #include <utils/configs.h>
    #include <commons/collections/dictionary.h>
    #include <dirent.h>
    #include <globales.h>
    #include <commons/string.h>
    #include <bloques.h>

    void levantar_diccionario_fcb(t_config*);
    void crear_fcb(char*);
    void actualizar_fcb(char*, char*, char*);

    // Getters 
    int obtener_tamanio(char*);
    int obtener_puntero_indirecto(char*);


    
    
#endif