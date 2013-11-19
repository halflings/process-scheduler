#include "../os-old/hw.h"
#include "../os-old/sched.h"
#include "../os-old/malloc.h"

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


//------------------------------------------------------------------------
int
start_kernel ( void )
{
  DISABLE_IRQ();
  init_hw();
  malloc_init((void *) HEAP_START);

  create_process(&processus_A, (void*) 0);
  create_process(&processus_B, (void*) 0);
  
  start_sched();
  
  return 0;
}
