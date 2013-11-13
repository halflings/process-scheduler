#ifndef SCHED_H
#define SCHED_H
#include <stdint.h>
#define STACK_SIZE 0xffff

typedef void (*func_t) (void*);
typedef enum {NEW,RUNNING,TERMINATED,WAITING} procState;

struct pcb_s {
    procState state;
    struct pcb_s* next;
    struct pcb_s* prev;
    
    func_t entry_point;
    
    uint32_t* sp;
    void* args; 
};


struct pcb_s* current_process;

void create_process(func_t f, void* args);
void ctx_switch();
void start_sched();

#endif