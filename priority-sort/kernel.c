#include "../os/sched.h"
#include "../os/sem.h"
#include "../os/hw.h"
#include "../os/malloc.h"

#define MAX_SIZE 8
int sorted[MAX_SIZE];
int size = 0;

struct sem_s *insert_sem;

void sort(void* args) {
    int value = (int) args;
         
    sem_down(insert_sem);
    sorted[size] = value;
    size++;
    sem_up(insert_sem);
}

//------------------------------------------------------------------------
int start_kernel ( void ) {
    malloc_init((void*) HEAP_START);
    init_priorities();
    
    // Initializing the elements
    int unsorted[MAX_SIZE];
    unsorted[0] = 6;
    unsorted[1] = 8;
    unsorted[2] = 6;
    unsorted[3] = 2;
    unsorted[4] = 4;
    unsorted[5] = 1;
    unsorted[6] = 6;
    unsorted[7] = 10;

    int i;
    for (i = 0; i < MAX_SIZE; i++) {
        create_process(sort, (void*) unsorted[i], 2 +  unsorted[i]);
    }

    sem_init(&insert_sem, 1);

    start_sched();

    return(0);
}
