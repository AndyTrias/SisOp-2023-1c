#include <bloques.h>

u_int32_t buscar_bloque_libre()
{
    int i, estado;
    for (i = 0; i < bitarray_get_max_bit(BITMAP_BLOQUES); i++)
    {

        estado = bitarray_test_bit(BITMAP_BLOQUES, i);
        log_info(LOGGER_FILE_SYSTEM, "Acceso a Bitmap - Bloque: %d - Estado: %d", i, estado);
        if (!estado)
        {
            bitarray_set_bit(BITMAP_BLOQUES, i);
            // msync(BITMAP_BLOQUES->bitarray, BITMAP_BLOQUES->size, MS_SYNC);

            return i;
        }
    }
    return -1;
}

void marcar_bloque_como_libre(uint32_t bloque)
{

    log_info(LOGGER_FILE_SYSTEM, "Acceso a Bitmap - Se marca al Bloque: %d como Libre", bloque);
    bitarray_clean_bit(BITMAP_BLOQUES, bloque);
    // msync(BITMAP_BLOQUES->bitarray, BITMAP_BLOQUES->size, MS_SYNC);
}

void asignar_bloques_al_puntero_indirecto(void *puntero, int cantidad_bloques_necesarios, int bloques_actuales)
{

    u_int32_t bloque_libre;

    // Como el primer bloque es el puntero directo es - 1
    int punteros_actuales = MAX(bloques_actuales -1, 0);
    for (int i = 0; i < cantidad_bloques_necesarios; i++)
    {
        bloque_libre = buscar_bloque_libre();
        if (bloque_libre == -1)
        {
            log_error(LOGGER_FILE_SYSTEM, "No hay bloques libres");
            return;
        }
        else
        {
            memcpy(puntero + sizeof(u_int32_t) * (punteros_actuales + i), &bloque_libre, sizeof(uint32_t));
        }
    }
}

void liberar_bloques_del_puntero_indirecto(void *puntero, int cantidad_bloques_a_liberar, int bloques_actuales)
{
    // Como el primer bloque es el puntero directo es - 1
    // Como es para leer me posiciono en el ultimo bloque - 1, que es donde empieza
    int posicion_a_leer = MAX(bloques_actuales - 2, 0);


    for (int i = 0; i < cantidad_bloques_a_liberar; i++)
    {
        int bloque = leer_puntero_indirecto(puntero, posicion_a_leer - i);
        marcar_bloque_como_libre(bloque);
    }
}


void* cargar_bloque_indirecto(char* nombre_archivo, void* archivo_de_bloques)
{
    // Hacer el retardo
    // Hacer el log
    return archivo_de_bloques + TAMANIO_BLOQUES * obtener_puntero_indirecto(nombre_archivo);
}


int leer_puntero_indirecto(void* puntero, int numero_de_bloque)
{
    int bloque_a_leer;
    memcpy(&bloque_a_leer, puntero + numero_de_bloque * sizeof(u_int32_t), sizeof(uint32_t));
    return bloque_a_leer;
}

char* leer_bloque(void* archivo_de_bloques, int bloque, int offset, int tamanio){
    char* bloque_a_leer = string_new();
    memcpy(bloque_a_leer, archivo_de_bloques + bloque * TAMANIO_BLOQUES + offset, tamanio);
    return bloque_a_leer;
}