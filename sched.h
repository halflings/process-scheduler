#ifndef SCHED_H
#define SCHED_H

#include "process.h"

void create_process(func_t f, void* args);
void yield();

#endif
