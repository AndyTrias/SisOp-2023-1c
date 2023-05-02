#ifndef PCB_H_
#define PCB_H_
    
   #include<commons/collections/list.h>
   #include<commons/temporal.h>
   
   typedef struct
    {
        t_ctx contexto;
        //tabla de segmento;
        int estimado_prox_rafaga;
        t_temporal* tiempo_desde_ult_ready; //cuando pasa a ready usar temporal_stop
        int tiempo_para_ready;//cuando pasa a ready usar  = temporal_gettime(tiempo_desde_ult_ready)
        t_list* archivos_abiertos; 
    } t_pcb;

  
#endif
