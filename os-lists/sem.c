#include "sem.h"
#include "malloc.h"
#include "sched.h"
#include "hw.h"

void sem_init(struct sem_s** sem, unsigned int val){
	*sem = (struct sem_s*) malloc_alloc(sizeof(struct sem_s));
	(*sem)->val = val;
	(*sem)->waiting = 0;
	(*sem)->queue = 0;
}

void sem_up(struct sem_s* sem) {

    DISABLE_IRQ();
	if (sem->waiting > 0) {
		// We take the first waiting process
		struct waiting_process* first_process = sem->queue;
		// Remove it from the top of the waiting list
		sem->queue = first_process->next;
		sem->waiting -= 1;

		// Put the process back in the active list
		put_back(first_process->process);

		// And free the allocated memory to its position in the queue
		malloc_free((uint32_t*) first_process);
	}
	else {
	    sem->val += 1;
	}
	ENABLE_IRQ();
}


void sem_down(struct sem_s* sem) {
    DISABLE_IRQ();
	
	if (sem->val <= 0) {
		// Initializing "wait_proc"
		struct waiting_process* wait_proc = (struct waiting_process*) malloc_alloc(sizeof(struct waiting_process));
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
        
        // TODO : Marche pas si c'est le dernier process de sa queue
        // ... 'va juste changer la valeur de current_process->schedule_queue
        move_process(current_process, &current_process->schedule_queue, waiting)
        
		current_process->state = WAITING;
		ENABLE_IRQ();
		
        // TODO : Schedule instead of doing a while
        while(current_process->state== WAITING){
            //DO NOTHING
        }
	}
	else {
	    sem->val -= 1;
		ENABLE_IRQ();
	}

}

void mtx_init(struct mtx_s** mutex){
	(*mutex) = (struct mtx_s*) malloc_alloc(sizeof(struct mtx_s));
	(*mutex)->owner = 0;
    sem_init( &( (*mutex)->sem_mtx ), 1);
	//mutex->pOwnerPid=getpid();
}
void mtx_lock(struct mtx_s* mutex){
    DISABLE_IRQ();
    mutex->owner = current_process;
    sem_down(mutex->sem_mtx);
	ENABLE_IRQ();
}
void mtx_unlock(struct mtx_s* mutex){
    DISABLE_IRQ();
    if (current_process == mutex->owner) {
        sem_up(mutex->sem_mtx);
    }
	ENABLE_IRQ();
}


