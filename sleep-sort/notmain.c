#include "../os/sched.h"
#include "../os/sem.h"

#DEFINE MAX_SIZE 10;
int sorted[MAX_SIZE];
int size = 0;

struct sem_s *insert_sem;

void sort(void* args) {
    int value = (int) args;
    sleep_proc(value*4);
         
    sem_down(insert_sem);
    sorted[size] = value;
    size++;
    sem_up(insert_sem);
}

//------------------------------------------------------------------------
int notmain ( void ) {

    // Initializing the elements
    int unsorted[5];
    unsorted[0] = 3;
    unsorted[1] = 1;
    unsorted[2] = 6;
    unsorted[3] = 2;
    unsorted[4] = 4;

    int i;
    for (i = 0; i < 5; i++) {
        create_process(sort, (void*) unsorted[i]);
    }

    sem_init(&insert_sem, 1);

    start_sched();

    return(0);
}
