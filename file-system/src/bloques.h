#ifndef BLOQUES_H
#define BLOQUES_H

    #include <globales.h>
    #include <string.h>
    #include <stdint.h>
    #include <commons/bitarray.h>
    #include <math.h>

    #include <sys/mman.h>

    u_int32_t buscar_bloque_libre();    
    void marcar_bloque_como_libre(uint32_t); 
    void asignar_bloques_al_puntero_indirecto(void *, int , int );
    void liberar_bloques_del_puntero_indirecto(void* , int , int );

#endif