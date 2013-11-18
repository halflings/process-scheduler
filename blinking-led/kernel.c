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
    led_off();
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

  //led_on();

  create_process(&processus_B, (void*) 0);
  create_process(&processus_A, (void*) 0);
  //create_process(&processus_B, (void*) 0);
  
  start_sched();

  //int tmp;
  //for ( ; ; ) {
  //  tmp++;
  //}  

  return 0;
}
