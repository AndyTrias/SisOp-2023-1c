#include "verificar_argumentos.h"

FILE* verificar_argumentos(int argc, char *argv[]) {
    // Verifico que se hayan ingresado los archivos de configuracion y de instrucciones
    if (argc < 3) {
        
        if (argc < 2) {
            log_error(logger_consola, "Debe ingresar el archivo de configuracion\n");
        }
        else {
            log_error(logger_consola, "Debe ingresar el archivo con las instrucciones\n");
        }
        
        return NULL;
    }

    FILE *f = fopen(argv[2], "r");
    if (f == NULL) {
        log_error(logger_consola, "No se pudo abrir el archivo de instrucciones");
        return NULL; 
    }

    return f;
}