#define MAX_PRIORITY 255

// Current process
struct pcb_s* current_process;

// Process queues
struct pcb_s* processes[MAX_PRIORITY];
struct pcb_s* sleeping_processes;



// In the scheduling algorithm

	// Decrementing "ticks" count for sleeping processes
	struct pcb_s* proc_iter = sleeping_processes;
	do {
        proc_iter->ticks -= 1;
        if (proc_iter->ticks <= 0) {
        	// Saving the pointer to the next sleeping process
        	struct pcb_s* next_process = proc_iter->next;

        	// Removing the process from the sleeping list
        	proc_iter->prev->next = proc_iter->next;
        	proc_iter->next->prev = proc_iter->prev;

        	// Putting the process back
            struct pcb_s* process_list = processes[proc_iter->priority];

            proc_iter->next = process_list;
            proc_iter->prev = process_list->prev;

            process_list->prev->next = proc_iter;
            process_list->prev = proc_iter;

            // Restoring the next sleepin process
            proc_iter = next_process;
        }
        else {	    
	    	proc_iter = proc_iter->next;
	    }
	}
	while (proc_iter != sleeping_processes);

	// Choosing the first process with the highest priority
	for (int i = 0; i < MAX_PRIORITY; i++) {
		if (processes[i] == 0) {
			continue;
		}

		current_process = processes[i];
		processes[i] = processes[i]->next;
		break;
	}
