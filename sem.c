#include "sem.h"

void sem_init(struct sem_s* sem, unsigned int val){
	sem = Allocate(sizeof(struct sem_s));
	sem->value = val;
	sem->waiting = 0;
	sem->queue = 0;
}

void sem_up(struct sem_s* sem) {
	if (sem->waiting > 0) {
		// We take the first waiting process
		waiting_process* first_process = sem->queue;
		// Remove it from the top of the waiting list
		queue = first_process->next;
		sem->waiting -= 1;

		// Put its state at "RUNNING"
		first_process->process->state = RUNNING;

		// And free the allocated memory to its position in the queue
        FreeAllocatedMemory((uint32_t*) first_process);
	}

	sem->val += 1;
}


void sem_down(struct sem_s* sem) {

	sem->val -= 1;

	if (sem->val < 0) {
		// Initializing "wait_proc"
		waiting_process* wait_proc = (struct waiting_process*) AllocateMemory(sizeof(waiting_process));
		wait_proc->process = current_process;
		wait_proc->next =  0;

		// If the waiting queue is empty, we put our process as the first (and only) waiting element
		if (sem->waiting == 0) {
			sem->last_process = wait_proc;
			sem->queue = wait_proc;
		}
		// Otherwise we put it at the end of the queue
		else {
			sem->last_process->next = wait_proc;
			sem->last_process = wait_proc;
		}

		sem->waiting += 1;

		current_process->state = WAITING;

		ctx_switch();
	}

}