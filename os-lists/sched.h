#ifndef SCHED_H
#define SCHED_H
#include <stdint.h>
#define STACK_SIZE 0xffff
#define MAX_PRIORITY 255

typedef void (*func_t) (void*);

struct pcb_s {
    struct pcb_s* next;
    struct pcb_s* prev;
    
    int ticks;
    
    func_t entry_point;
    
    struct pcb_s* schedule_queue;

    char* stack_base; 
    uint32_t* sp;
    void* args; 
};


struct pcb_s* current_process;

struct pcb_s* processes[MAX_PRIORITY];
struct pcb_s* sleeping_processes;
struct pcb_s* waiting_processes;
struct pcb_s* terminated;

// Utility functions
void move_process(struct pcb_s* process, struct pcb_s* destination);
void schedule();

// Public
void create_process(func_t f, void* args);
void ctx_switch();
void start_sched();
void yield();
void start_current_process();

void sleep_proc(int ticks);

#endif
