#include "process.h"
#include "dispatcher.h"
#include "sched.h"


void funcA()
{
  int cptA = 0;

  while ( 1 ) {
    cptA ++;
    yield();
  }
}

void funcB()
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
