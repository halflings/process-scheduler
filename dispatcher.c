#include "dispatcher.h"


// À noter: __asm volatile ("push [r0_r12,lr]");
// (ou la même avec pop à la place de push)

void start_current_process() {
    current_process->pc();
}

void ctx_switch(struct pcb_s* ctx){
    
   // struct pcb_s* context = ctx;
    

    
    // On enregistre le contexte courant
    // __asm("mov %0, lr" : "=r"(current_process->pc));
    __asm("mov %0, sp" : "=r"(current_process->sp));
    
    current_process = ctx;
    
        // On stocke les valeurs des registres dans  la pile 
    __asm volatile ("push {r0-r12,lr}");
    
    if (current_process->state == NEW) {
        current_process->state = RUNNING;
        start_current_process();
        current_process->state = DYING;
        if(current_process->next!=current_process){
            current_process = current_process->next;
        } else {
            
      }
        
    }
    

        
    // On recupère les valeurs enregistrées des registres depuis la pile à partir du nouveau sp
    __asm volatile ("pop {r0-r12,lr}");
    
        // On fixe le stack pointer du nouveau contexte
    __asm("mov sp, %0" : : "r"(current_process->sp));
    
    // On fixe la valeur de retour au début de la routine du nouveau contexte
    // __asm("mov lr, %0" : : "r"(current_process->pc));
}
