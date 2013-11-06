#ifndef SEM_H
#define SEM_H

struct waiting_process {
	pcb_s* process;
	waiting_process** next;
};

struct sem {
    int value;

    int waiting;
    waiting_process* queue;
    waiting_process* last_process;
};

void sem_init(struct sem_s* sem, unsigned int val);

#endif
