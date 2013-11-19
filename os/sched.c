#include "sched.h"
#include "malloc.h"
#include "hw.h"

void start_sched() {
	DISABLE_IRQ();
	init_hw();
	set_tick_and_enable_timer();
	ENABLE_IRQ();

	// Waiting for the first ctx_switch
	int tmp = 0;
	while (1) {
	    tmp++;
	}
}

void create_process(func_t f, void* args, int priority) {

    if (priority == COLLABORATIVE) {
        priority = 0;
    }
    else if (priority == UNPRIORITIZED) {
        priority = 1;
    }
    else if (priority <= 1) {
        priority = 1;
    }
    else if (priority >= MAX_PRIORITY) {
        priority = MAX_PRIORITY - 1;
    }
    
    // On initialise le PCB
    struct pcb_s* pcb = (struct pcb_s*) malloc_alloc(sizeof(struct pcb_s));
    
    // init pcb
    pcb->state = NEW;
    pcb->args = args;
    pcb->entry_point = f;
    pcb->priority = priority;
    
    pcb->stack_base = malloc_alloc(STACK_SIZE);
    
    //pcb->sp = ((uint32_t*) (pcb->stack_base + STACK_SIZE)) - 1;
    pcb->sp = (uint32_t*) pcb->stack_base + (STACK_SIZE / sizeof(uint32_t)) - 1;
    
    pcb->ticks = 0;
    
    if (current_process == 0) {
        current_process = pcb;
    }
    
    if (processes[priority] == 0) {
        pcb->next = pcb;
        pcb->prev = pcb;
    
        processes[priority] = pcb;
    }
    else {
        struct pcb_s* process_head = processes[priority];
        
        pcb->prev = process_head->prev;
        pcb->next = process_head;
    
        process_head->prev->next = pcb;
        process_head->prev = pcb;
    }
    
    *(pcb->sp) = 0x53;
    pcb->sp--;
    *(pcb->sp) = (unsigned int) &start_current_process;

}

void
start_current_process()
{
  current_process->state = READY;
  current_process->entry_point(current_process->args);
  current_process->state = TERMINATED;
  yield();
}

void schedule() {

    struct pcb_s* next_process = 0;

    while (next_process == 0) {
	    // Choosing the first process with the highest priority
	    int i;
	    for (i = 0; i < MAX_PRIORITY; i++) {
	        // If there's no process for this priority, we continue to the next priority
		    if (processes[i] == 0) {
			    continue;
		    }
		
		    // Otherwise, we go throught all the processes of the current priority
		    // If we didn't choose the next process yet and we find a READY process => it'll be the next process 
		    // We also update the ticks of SLEEPing processes and put awaken processes to the READY state
		    struct pcb_s* proc_init = processes[i];
	        struct pcb_s* proc_iter = processes[i];
	        do {
	            int ok_process = (proc_iter->state == READY || proc_iter->state == NEW);
	            if (next_process == 0 && ok_process) {
	                next_process = proc_iter;
	                processes[i] = next_process->next;
	            }
	            else if (proc_iter->state == SLEEPING) {
                    proc_iter->ticks -= 1;
                    if (proc_iter->ticks <= 0) {
                    	proc_iter->state = READY;
                    }
                }
                else if (proc_iter->state == TERMINATED) {
                    	struct pcb_s* terminated_proc = proc_iter;
                    
                    	terminated_proc->prev->next = terminated_proc->next;
                    	terminated_proc->next->prev = terminated_proc->prev;
                    	
                    	int last_process = (terminated_proc == terminated_proc->next);
         
                    	malloc_free((char*) terminated_proc->stack_base);
	                    malloc_free((char*) terminated_proc);
	                    
	                    if (last_process) {
	                        processes[i] = 0;
	                        break;
	                    }
	            }

            	proc_iter = proc_iter->next;
	        }
	        while (proc_iter != proc_init); // do-while loop through processes of the same priority
	        
	    } // for-loop through priorities
	}
	
	
	current_process = next_process;
}

void init_priorities() {
    // Initializing process queues
    int i;
    for (i = 0; i < MAX_PRIORITY; i ++) {
        processes[i] = 0;
    }
}

void  __attribute__((naked)) ctx_switch() {

    __asm volatile("sub lr, lr, #4");
    __asm volatile("srsdb sp!, 0x13");

    __asm volatile("cps #0x13");

    // Saving the current context
    __asm volatile ("push {r0-r12,lr}");

    DISABLE_IRQ();

    __asm("mov %0, sp" : "=r"(current_process->sp));

    // Switching to the next process
    if (current_process->priority != 0 || current_process->state != READY) {
        schedule();
        __asm("mov sp, %0" : : "r"(current_process->sp));
    }

    // If the process is new, we execute its entry-point
    if (current_process->state == NEW) {
        current_process->state = READY;

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
 
 
void yield() {
    // Saving the current context
    __asm volatile ("push {r0-r12,lr}");
    
    DISABLE_IRQ();
    
    __asm("mov %0, sp" : "=r"(current_process->sp));

    // Switching to the next process
    schedule();
    __asm("mov sp, %0" : : "r"(current_process->sp));
         
    
    // If the process is new, we execute its entry-point
    if (current_process->state == NEW) {
        current_process->state = READY;

        set_tick_and_enable_timer();
        ENABLE_IRQ();

        current_process->entry_point(current_process->args);
        DISABLE_IRQ();
        current_process->state = TERMINATED;

        yield();
    }
    else {
        // Restoring the context
        __asm volatile ("pop {r0-r12,lr}");
    }
    
}

 

void sleep_proc(int ticks) {

    current_process->state = SLEEPING;
    current_process->ticks = ticks;
    
    while (current_process->state == SLEEPING) {
    }
}



