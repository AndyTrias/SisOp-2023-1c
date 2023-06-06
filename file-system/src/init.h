#ifndef INIT_FILE_SYSTEM_H
#define INIT_FILE_SYSTEM_H

    #include <utils/conexiones.h>
    #include <utils/configs.h>
    #include <commons/bitarray.h>
    #include <fcntl.h>
    #include <sys/mman.h>
    #include <globales.h>



    void inicializar_conexiones(int *, t_config* );
    void inicializar_archivos(t_config* );
    t_bitarray* levantar_bitmap(int , char* );
    t_config* levantar_superbloque(t_config* config);




#endif