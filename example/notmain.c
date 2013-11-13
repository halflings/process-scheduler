#include "../os/sched.h"
#include "../os/sem.h"

void funcA(void* args) {
    int cptA = 0;
    while ( cptA < 10 ) {
    cptA ++;
    }
}

void funcB(void* args) {
    int cptB = 1;

    while ( 1 ) {
    cptB += 2 ;
    }
}

//------------------------------------------------------------------------

int cpt = 0;
struct sem_s *sem_test;

void funcOne(void* args) {
    while (1==1){
        int i;
        sem_down(sem_test);
        for (i=0; i<1000000;i++){
            cpt++;
        }
        sem_up(sem_test);
    }    
}


void funcTwo(void* args) {
    while (1==1){
        int i =0;
        sem_down(sem_test);
        for (i=0; i<1000000;i++){
            cpt++;
            }
        sem_up(sem_test);
    }              
}

//------------------------------------------------------------------------
int notmain ( void ) {
    create_process(funcOne, (void*) 0);
    create_process(funcTwo, (void*) 0);

    sem_init(&sem_test, 1);
    

    start_sched();

    /* Pas atteignable vues nos 2 fonctions */
    return(0);
}
