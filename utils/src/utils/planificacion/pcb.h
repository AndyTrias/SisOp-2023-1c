#ifndef PCB_H_
#define PCB_H_
    
   #include<commons/collections/list.h>
   #include<commons/temporal.h>

   
typedef struct {
        char AX[4];
        char BX[4];
        char CX[4];
        char DX[4];
        
        char EAX[8];
        char EBX[8];
        char ECX[8];
        char EDX[8];

        char RAX[16];
        char RBX[16];
        char RCX[16];
        char RDX[16];
} t_registros;


   typedef struct
    {
        int PID;
        t_list* instrucciones; // inicializo el pcb con una lista de instrucciones vacia
        int program_counter;
        t_registros registros; 
    } t_ctx;

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
