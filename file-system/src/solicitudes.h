#ifndef SOLICITUDES_H
#define SOLICITUDES_H
    
   #include <utils/mensajes.h>
   #include <utils/planificacion/pcb.h>
   #include <utils/configs.h>

   
   #include <commons/string.h>
   
   #include <globales.h>
   #include <fcb.h>


   void atender_solicitudes(int, t_parametros_variables *);
   int buscar_bloque_libre();


    
    
#endif