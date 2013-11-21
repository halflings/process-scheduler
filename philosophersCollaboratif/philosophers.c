#include "../os/hw.h"
#include "../os/sched.h"
#include "../os/malloc.h"

#define NB 5

extern struct pcb_s idle;
extern struct pcb_s* ready_queue;
extern struct pcb_s* current_process;

void philosopher(void* args) {
	int pos = (int) args;

    while (1) {   
		
		if (pos == 0)
			led_on();
			
		//sleep_proc(5);
		
		if (pos == 0)
			led_off();
			
		yield();    

    }
}



//------------------------------------------------------------------------

int start_kernel ( void ) {

	malloc_init((void *) HEAP_START);
	current_process = &idle;
	idle.next = ready_queue;
	
    int j;
    for(j = 0; j < NB; j++) {

        create_process(philosopher, 512, (void*) j);

    }
	yield();

    return(0);

}
