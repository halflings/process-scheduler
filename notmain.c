#include "sched.h"

void funcA(void* args)
{
  int cptA = 0;


  cptA ++;
  yield();
  
}

void funcB(void* args)
{
  int cptB = 1;

  while ( 1 ) {
    cptB += 2 ;
    yield();
  }
}

//------------------------------------------------------------------------
int notmain ( void )
{
  create_process(funcA, (void*) 0);
  create_process(funcB, (void*) 0);
  
  yield();

  /* Pas atteignable vues nos 2 fonctions */
  return(0);
}
