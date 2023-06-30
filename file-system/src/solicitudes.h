#ifndef SOLICITUDES_H
#define SOLICITUDES_H
    
   #include <utils/mensajes.h>
   #include <utils/planificacion/pcb.h>
   #include <utils/configs.h>
   
   #include <commons/string.h>
   
   #include <globales.h>
   #include <fcb.h>


   #include <sys/mman.h>


   void atender_solicitudes(int, t_parametros_variables *);
   void agrandar_archivo(void* archivo_de_bloques, int necesarios, int actuales, bool nuevo_archivo);
   void achicar_archivo(void* archivo_de_bloques, int necesarios, int actuales, bool borrar_todo);

     
#endif