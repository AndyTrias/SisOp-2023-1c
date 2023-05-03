#include "recibir_consolas.h"

void recibir_consolas(int socket_servidor){
    
    log_info(LOGGER_KERNEL, "Esperando conexiones de consolas...");
   
    while(1){
        conectar_consola(socket_servidor);
        sem_post(&CONSOLA_CONECTADA);
    }
}