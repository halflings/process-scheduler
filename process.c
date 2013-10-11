#include "process.h"
#include "allocateMemory.h"

void init_pcb(struct pcb_s* pcb, func_t f,void* args, unsigned int stack_size) { 
    pcb->state = NEW;
    pcb->args = args;
    pcb->pc = f;
    
    pcb->sp = (uint32_t*) (AllocateMemory(stack_size) + (stack_size - 1) * 4);
}
