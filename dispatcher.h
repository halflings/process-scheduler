#ifndef DISPATCHER_H
#define DISPATCHER_H

#include "process.h"

void start_current_process();
void ctx_switch(struct pcb_s* ctx);

#endif
