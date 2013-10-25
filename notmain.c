#include "sched.h"

void funcA(void* args)
{
  int cptA = 0;
  while ( 1 ) {
	cptA ++;
   }

}

void funcB(void* args)
{
  int cptB = 1;

  while ( 1 ) {
    cptB += 2 ;
  }
}

//------------------------------------------------------------------------
int notmain ( void )
{
  create_process(funcA, (void*) 0);
  create_process(funcB, (void*) 0);
  
  start_sched();

  /* Pas atteignable vues nos 2 fonctions */
  return(0);
}
