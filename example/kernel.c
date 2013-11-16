#include "../os/sched.h"
#include "../os/sem.h"
#include "../os/hw.h"
#include "../os/malloc.h"
//------------------------------------------------------------------------

int cpt = 0;
struct sem_s *sem_test;

void funcOne(void* args) {
    while (1==1){
        int i;
        sem_down(sem_test);
        for (i=0; i<10000000;i++){
            cpt++;
        }
	sleep_proc(4);
        sem_up(sem_test);
    }    
}


void funcTwo(void* args) {
    while (1==1){
        int i =0;
        sem_down(sem_test);
        for (i=0; i<10000000;i++){
            cpt++;
        }
	    sleep_proc(1);
        sem_up(sem_test);
    }              
}

//------------------------------------------------------------------------
int start_kernel ( void ) {
    malloc_init((void *) HEAP_START);

    create_process(funcOne, (void*) 0);
    create_process(funcTwo, (void*) 0);

    sem_init(&sem_test, 1);
    
    start_sched();

    /* Pas atteignable vues nos 2 fonctions */
    return(0);
}
