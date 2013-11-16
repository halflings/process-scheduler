#ifndef SCHED_H
#define SCHED_H
#include <stdint.h>
#define STACK_SIZE 0xbbbb

typedef void (*func_t) (void*);
typedef enum {NEW,RUNNING,TERMINATED,WAITING,SLEEPING} procState;

struct pcb_s {
    procState state;
    struct pcb_s* next;
    struct pcb_s* prev;
    
    int ticks;
    
    func_t entry_point;

    char* stack_base; 
    uint32_t* sp;
    void* args; 
};


struct pcb_s* current_process;

void create_process(func_t f, void* args);
void ctx_switch();
void start_sched();

void sleep_proc(int ticks);

#endif
