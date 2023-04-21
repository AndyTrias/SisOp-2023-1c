#ifndef PCB_H_
#define PCB_H_
    
   #include<commons/collections/list.h>
   #include<commons/temporal.h>
   
   typedef struct
    {
        int PID;
        t_list* instrucciones; // inicializo el pcb con una lista de instrucciones vacia
        int program_counter;
        //t_registros cpu;
        //tabla de segmento
        int estimado_prox_rafaga;
        t_temporal* tiempo_desde_ult_ready; //cuando pasa a ready usar temporal_stop
        int tiempo_para_ready;//cuando pasa a ready usar  = temporal_gettime(tiempo_desde_ult_ready)
        t_list* archivos_abiertos; 

        

    } t_pcb;

#endif
