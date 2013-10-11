#ifndef PROCESS_H
#define PROCESS_H

#include <stdint.h>
#define STACK_SIZE 0xffff

typedef void (*func_t) ( void);
typedef enum {NEW,RUNNING,DYING} procState;

struct pcb_s {
        
    procState state;
    struct pcb_s* next;
    struct pcb_s* prev;
    
    func_t pc;
    
    uint32_t* sp;
    void* args; 
};


struct pcb_s* current_process;

void init_pcb(struct pcb_s* pcb, func_t f,void* args, unsigned int stack_size);

#endif
