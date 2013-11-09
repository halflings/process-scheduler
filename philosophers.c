#include "sched.h"
#include "sem.h"

#define NB 5
#define MAX 10
struct sem_s forks[NB];

void philosopher(void* args) {
    int actions = 0;
    int pos = (int) args;

    int left = i - 1;
    if (left < 0)
        left = NB-1;

    int right = i + 1;
    if (right == NB)
        right = 0;

    int first = min(left,right);
    int second = max(left,right);
    while ( actions < 10) {   
        sem_down(&forks[first]);
        sem_down(&forks[second]);
        actions ++;
        sem_up(&forks[first]);
        sem_up(&forks[second]);
    }

}

//------------------------------------------------------------------------
int notmain ( void ) {
    int i;
    for(i = 0; i < NB; i++) {
        create_process(philosopher, (void*) i);
        sem_init(&forks[i], 1);
    }
    start_sched();
    return(0);
}
