#include "hw.h"
#include "music.h"
#include "sched.h"

extern void play_music();

//------

void
processus_A( void* args )
{
  int cptA = 0;

  while ( 1 ) {
    cptA ++;
  }
}

void
processus_B( void* args )
{
  int cptB = 1;

  while ( 1 ) {
    cptB += 2 ;
  }
}


//------------------------------------------------------------------------
int
notmain ( void )
{
	malloc_init((void *) HEAP_START);

	music_init();

	create_process(processus_A, 128);
	create_process(processus_A, 128);

	create_process(play_music, 2048);

	create_process(processus_B, 128);
	create_process(processus_B, 128);

	start_sched();

	return(0);
}

