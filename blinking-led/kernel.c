#include "../os/hw.h"
#include "../os/sched.h"
#include "../os/malloc.h"

void
processus_A()
{
  int i = 0;
  int j = 0;
  while (j < 10) {
    led_on();
    while ( i++ < 2000000);
    i = 0;
    j++;
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


//------------------------------------------------------------------------
int
start_kernel ( void )
{
    malloc_init((void *) HEAP_START);
    init_priorities();

    create_process(&processus_A, (void*) 0, 0);
    create_process(&processus_B, (void*) 0, 100);
  
    start_sched();
  
    return 0;
}

