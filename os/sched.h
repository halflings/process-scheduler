#ifndef SCHED_H
#define SCHED_H
#include <stdint.h>
#define STACK_SIZE 0xffff
#define MAX_PRIORITY 255


typedef void (*func_t) (void*);
typedef enum {NEW,READY,TERMINATED,WAITING,SLEEPING} procState;

struct pcb_s {
    procState state;
    struct pcb_s* next;
    struct pcb_s* prev;
    
    int ticks;
    int priority;
    
    func_t entry_point;

    char* stack_base; 
    uint32_t* sp;
    void* args; 
};


struct pcb_s* current_process;

// Process queues
struct pcb_s* processes[MAX_PRIORITY];

void create_process(func_t f, void* args,int priority);
void ctx_switch();
void start_sched();
void yield();
void schedule();
void start_current_process();
void init_priorities();

void sleep_proc(int ticks);

#endif

