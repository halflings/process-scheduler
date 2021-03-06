#include "../os/hw.h"
#include "../os/sched.h"
#include "../os/malloc.h"

void
processus_A()
{
  int i = 0;
  while (1) {
    led_on();
    while ( i++ < 2000000);
    i = 0;
  }
}

void
processus_B()
{
  int i = 0;

  while (1) {
    led_off();
    while ( i++ < 2000000);
    i = 0;
  }
}

void 
processus_C()
{
    int i = 0;
    while(1)
    {
        i++;
    };
}

void 
processus_D()
{
    int i = 0;
    for(i = 0; i<5; i++)
    {
        create_process(&processus_C, (void*) 0, UNPRIORITIZED);
    };
}


//------------------------------------------------------------------------
int
start_kernel ( void )
{
    malloc_init((void *) HEAP_START);
    init_priorities();

    create_process(&processus_A, (void*) 0, UNPRIORITIZED);
    create_process(&processus_B, (void*) 0, UNPRIORITIZED);
    create_process(&processus_D, (void*) 0, UNPRIORITIZED);
   
    start_sched();
  
    return 0;
}

