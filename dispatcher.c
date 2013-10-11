#include "dispatcher.h"


// À noter: __asm volatile ("push [r0_r12,lr]");
// (ou la même avec pop à la place de push)

void start_current_process() {
    current_process->pc();
}

void ctx_switch(struct pcb_s* ctx){
    // On enregistre le contexte courant
    __asm("mov %0, lr" : "=r"(current_process->pc));
    __asm("mov %0, sp" : "=r"(current_process->sp));

    // On stocke les valeurs des registres dans la pile 
    __asm volatile ("stmda sp, {r0-r12}");
    
    current_process = ctx;
    
    if (ctx->state == NEW) {
        ctx->state = RUNNING;
        start_current_process();
    }
    
    // On fixe la valeur de retour au début de la routine du nouveau contexte
    __asm("mov lr, %0" : : "r"(ctx->pc));
    
    // On fixe le stack pointer du nouveau contexte
    __asm("mov sp, %0" : : "r"(ctx->sp));
    
    // On recupère les valeurs enregistrées des registres depuis la pile à partir du nouveau sp
    __asm volatile ("ldmib sp,  {r0-r12}");
}
