#include "sched.h"
#include "allocateMemory.h"
#include "hw.h"


void create_process(func_t f, void* args) {
    // On initialise le PCB
    struct pcb_s* pcb = (struct pcb_s*) AllocateMemory(sizeof(struct pcb_s));
    
    // init pcb
    pcb->state = NEW;
    pcb->args = args;
    pcb->entry_point = f;
    pcb->sp = (uint32_t*) (AllocateMemory(STACK_SIZE) + (STACK_SIZE - 1) * 4);
    
    if (current_process == 0) {
        pcb->next = pcb;
        pcb->prev = pcb;
        
        current_process = pcb;
    }
    else {
        // On met à jour les pointeurs du PCB
        pcb->prev = current_process->prev;
        pcb->next = current_process;
        
        // On place le PCB à la fin de notre chaîne
        (current_process->prev)->next = pcb;
        current_process->prev = pcb;
    }
}

void  __attribute__((naked)) ctx_switch() {
    __asm("sub lr, lr, #4");
    __asm("srsdb sp!, #19");
    __asm("cps #19");


    // On enregistre le contexte courant
    __asm volatile ("push {r0-r12,lr}");
    __asm("mov %0, sp" : "=r"(current_process->sp));

    while (current_process->next->state == DYING) {
        struct pcb_s* terminated_proc = current_process->next;
        
        current_process->next = terminated_proc->next;
        current_process->next->prev = current_process;
        
        FreeAllocatedMemory((uint32_t*) terminated_proc);
        
        current_process = current_process->next;
        __asm("mov sp, %0" : : "r"(current_process->sp));
    }

    current_process = current_process->next;
    __asm("mov sp, %0" : : "r"(current_process->sp));
	 

    
    if (current_process->state == NEW) {
        current_process->state = RUNNING;
        set_next_tick_and_enable_timer_irq();
        ENABLE_IRQ();
        current_process->entry_point(current_process->args);
        DISABLE_IRQ();
        current_process->state = DYING;
        ctx_switch();
    }
    else {
        set_next_tick_and_enable_timer_irq();
        // On recupère les valeurs enregistrées des registres depuis la pile à partir du nouveau sp
        __asm volatile ("pop {r0-r12,lr}");

    }
    
    __asm("rfefd sp!");
    ENABLE_IRQ();
}
 
void start_sched() {
	DISABLE_IRQ();
	init_hw();
    set_next_tick_and_enable_timer_irq();
	ENABLE_IRQ();
}




