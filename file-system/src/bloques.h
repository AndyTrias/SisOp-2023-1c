#ifndef BLOQUES_H
#define BLOQUES_H

    #include <globales.h>
    #include <string.h>
    #include <stdint.h>
    #include <commons/bitarray.h>
    #include <math.h>
    #include <fcb.h>

    #include <sys/mman.h>

    #define MAX(x, y) (((x) > (y)) ? (x) : (y))
    #define MIN(x, y) (((x) < (y)) ? (x) : (y))

    u_int32_t buscar_bloque_libre();    
    void marcar_bloque_como_libre(uint32_t);
    void asignar_bloques_al_puntero_indirecto(void *, int, int);
    void liberar_bloques_del_puntero_indirecto(void* , int , int);
    void* cargar_bloque_indirecto(char* nombre_archivo, void* archivo_de_bloques);
    int leer_puntero_indirecto(void* puntero, int numero_de_bloque);
    char* leer_bloque(void* archivo_de_bloques, int bloque, int offset, int tamanio);


#endif