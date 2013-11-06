#ifndef SEM_H
#define SEM_H
#include "sched.h"

struct waiting_process {
	struct pcb_s* process;
	struct waiting_process* next;
};

struct sem_s {
    int val;

    int waiting;
    struct waiting_process* queue;
    struct waiting_process* last_process;
};

void sem_init(struct sem_s* sem, unsigned int val);
void sem_down(struct sem_s* sem);
void sem_up(struct sem_s* sem);
#endif
