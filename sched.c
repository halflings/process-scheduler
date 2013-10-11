#include "sched.h"

#include "dispatcher.h"
#include "allocateMemory.h"

void create_process(func_t f, void* args) {
    // On initialise le PCB
    struct pcb_s* pcb = (struct pcb_s*) AllocateMemory(sizeof(struct pcb_s));
    init_pcb(pcb, f, args, STACK_SIZE);
    
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
    }
}

void yield() {
    // Le positionnement du next est déjà fait dans create_process...
    ctx_switch(current_process->next);
}
