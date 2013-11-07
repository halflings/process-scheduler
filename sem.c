#include "sem.h"
#include "allocateMemory.h"
#include "sched.h"

void sem_init(struct sem_s* sem, unsigned int val){
	sem = (struct sem_s*) AllocateMemory(sizeof(struct sem_s));
	sem->val = val;
	sem->waiting = 0;
	sem->queue = 0;
}

void sem_up(struct sem_s* sem) {
	if (sem->waiting > 0) {
		// We take the first waiting process
		struct waiting_process* first_process = sem->queue;
		// Remove it from the top of the waiting list
		sem->queue = first_process->next;
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
		struct waiting_process* wait_proc = (struct waiting_process*) AllocateMemory(sizeof(struct waiting_process));
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

void mtx_init(struct mtx_s* mutex){
	mutex = (struct mtx_s*) AllocateMemory(sizeof(struct mtx_s));
	mutex->iIsLocked = 0;
	mutex->iWaiting = 0;
	mutex->queue = 0;
	//mutex->pOwnerPid=getpid();
}
void mtx_lock(struct mtx_s* mutex){
	

}
void mtx_unlock(struct mtx_s* mutex){
}


