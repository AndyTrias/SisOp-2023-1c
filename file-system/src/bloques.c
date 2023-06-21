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
            return i;
        }
    }
    return -1;
}

void marcar_bloque_como_libre(uint32_t bloque)
{

    log_info(LOGGER_FILE_SYSTEM, "Acceso a Bitmap - Se marca al Bloque: %d como Libre", bloque);
    bitarray_clean_bit(BITMAP_BLOQUES, bloque);
}

void asignar_bloques_al_puntero_indirecto(void *puntero, int cantidad_bloques_necesarios, int tamanio_archivo)
{

    // calculo la cantidad de punteros que tiene ese bloques para ir a esa posicion
    int posicion_a_anotar = (tamanio_archivo - 1) / TAMANIO_BLOQUES;

    u_int32_t bloque_libre;
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
            memcpy((posicion_a_anotar + i) * sizeof(u_int32_t) + puntero, &bloque_libre, sizeof(uint32_t));
        }
    }
}

void liberar_bloques_del_puntero_indirecto(void *puntero, int cantidad_bloques_a_liberar, int tamanio_archivo)
{

    int posicion_a_leer = (tamanio_archivo - 1) / TAMANIO_BLOQUES - 1;

    // NO EXISTE MAX EN C
    if (posicion_a_leer < 0){
        posicion_a_leer = 0;
    }

    u_int32_t bloque_actual;
    for (int i = 0; i < cantidad_bloques_a_liberar; i++)
    {
        memcpy(&bloque_actual, puntero + (posicion_a_leer - i) * sizeof(u_int32_t) , sizeof(uint32_t));
        marcar_bloque_como_libre(bloque_actual);
    }
}