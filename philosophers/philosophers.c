#include "../os/hw.h"
#include "../os/sched.h"
#include "../os/malloc.h"
#include "../os/sem.h"

#define NB 5
#define ITERA 10

#define MIN(a,b) a < b ? a : b
#define MAX(a,b) a > b ? a : b

struct sem_s * forks[NB];

void philosopher(void* args) {

    int pos = (int) args;

    int left = pos;
    int right = pos + 1;

    if (right == NB) {
        right = 0;
	}
	
    int first = MIN(left,right);

    int second = MAX(left,right);

    while (1) {   

        sem_down(forks[first]);

        sem_down(forks[second]);
		
		if (pos == 0)
			led_on();
			
		sleep_proc(5);
		
		if (pos == 0)
			led_off();

        sem_up(forks[first]);

        sem_up(forks[second]);

    }



}



//------------------------------------------------------------------------

int start_kernel ( void ) {

	malloc_init((void *) HEAP_START);
	
    int j;
    for(j = 0; j < NB; j++) {

        create_process(philosopher, (void*) j);
        sem_init(&forks[j], 1);

    }

    start_sched();

    return(0);

}
