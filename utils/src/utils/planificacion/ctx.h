#ifndef CTX
#define CTX_H_
    
   #include<commons/collections/list.h>
   #include<commons/temporal.h>

typedef struct{
        char[4] registro_AX;
        char[4] registro_BX;
        char[4] registro_CX;
        char[4] registro_DX;
        
        char[8] registro_EAX;
        char[8] registro_EBX;
        char[8] registro_ECX;
        char[8] registro_EDX;

        char[16] registro_RAX;
        char[16] registro_RBX;
        char[16] registro_RCX;
        char[16] registro_RDX;
   } t_registros;

   typedef struct
    {
        int PID;
        t_list* instrucciones; // inicializo el pcb con una lista de instrucciones vacia
        int program_counter;
        t_registros cpu; 
    } t_ctx;
   
#endif