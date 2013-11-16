#include "sched.h"
#include "malloc.h"
#include "hw.h"

void start_sched() {
	DISABLE_IRQ();
	init_hw();
	set_tick_and_enable_timer();
	ENABLE_IRQ();
}

void create_process(func_t f, void* args) {
    // On initialise le PCB
    struct pcb_s* pcb = (struct pcb_s*) malloc_alloc(sizeof(struct pcb_s));
    
    // init pcb
    pcb->state = NEW;
    pcb->args = args;
    pcb->entry_point = f;
    
    pcb->stack_base = malloc_alloc(STACK_SIZE);
    pcb->sp = ((uint32_t*) (pcb->stack_base + STACK_SIZE)) - 1;
    pcb->ticks = 0;
    
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

    *(pcb->sp) = 0x53;
    pcb->sp--;
}

void  __attribute__((naked)) ctx_switch() {
    __asm volatile("sub lr, lr, #4");
    __asm volatile("srsdb sp!, 0x13");

    __asm volatile("cps #0x13");

    // Saving the current context
    __asm volatile ("push {r0-r12}");


    DISABLE_IRQ();

    __asm("mov %0, sp" : "=r"(current_process->sp));

    // Switching to the next process
    current_process = current_process->next;
    __asm("mov sp, %0" : : "r"(current_process->sp));
         
    while (current_process->state == TERMINATED || current_process->state == WAITING) {
	if (current_process->state == TERMINATED) {
        	struct pcb_s* terminated_proc = current_process;
        
        	terminated_proc->prev->next = terminated_proc->next;
        	terminated_proc->next->prev = terminated_proc->prev;
        
    		current_process = current_process->prev;

        	malloc_free((char*) terminated_proc->stack_base);
		malloc_free((char*) terminated_proc);
	}
        
        current_process = current_process->next;
        __asm("mov sp, %0" : : "r"(current_process->sp));
    }  

	// Decrementing "ticks" count for sleeping processes
    struct pcb_s* proc_iter = current_process;
    do {
    	if (proc_iter->state == SLEEPING) {
	        proc_iter->ticks -= 1;
	        if (proc_iter->ticks <= 0) {
	            proc_iter->state = RUNNING;
            }
        }
        
        proc_iter = proc_iter->next;
    }
    while (proc_iter != current_process);
    
    // If the process is new, we execute its entry-point
    if (current_process->state == NEW) {
        current_process->state = RUNNING;
        set_tick_and_enable_timer();
	ENABLE_IRQ();
        current_process->entry_point(current_process->args);
        DISABLE_IRQ();
        current_process->state = TERMINATED;
        ctx_switch();
    }
    else {
        set_tick_and_enable_timer();
	// Restoring the context
        __asm volatile ("pop {r0-r12,lr}");

    }
    
    // Cleaning up after ctx_switch's  execution
    __asm("rfefd sp!");
    ENABLE_IRQ();
}
 

void sleep_proc(int ticks) {

    current_process->state = SLEEPING;
    current_process->ticks = ticks;
    
    while (current_process->state == SLEEPING) {
    }
}


