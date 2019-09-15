
#include <string.h>

#include "pcb.h" 
#include "mpx_supt.h"


queue ready_queue = {.head = NULL, .tail = NULL, .count = 0};
queue blocked_queue = {.head = NULL, .tail = NULL, .count = 0};
queue suspended_ready_queue = {.head = NULL, .tail = NULL, .count = 0};
queue suspended_blocked_queue = {.head = NULL, .tail = NULL, .count = 0};

PCB* allocate_pcb(){
	
	PCB *pcb = sys_alloc_mem(sizeof(pcb));
	
	return pcb;
}

PCB* setup_pcb(char *name, int pclass, int priority){ // pclass refers to process class (system or user)

	PCB *new_pcb = allocate_pcb();		// allocates memory to the new PCB
	
	if(strlen(name) <8){	// Process name must be at least 8 characters
		strcpy(new_pcb -> name , name);		// sets the new process's name to name parameter
	}else{
		return NULL; // TODO ERROR CODE
	}
	if(priority >= 0 && priority <= 9){		// priority must be a number between 0 and 9
		new_pcb -> priority = priority;		// sets the new process's priority to priority parameter
	}else{
		return NULL; // TODO ERROR CODE
	}
	if(pclass == 0 || pclass ==1){			// process class must be either 0 for system, or 1 for user process
		new_pcb -> process_class = pclass;	// sets the new process's class to the pclass parameter
	}else{
		return NULL; // TODO ERROR CODE
	}

	return new_pcb;
}

int free_pcb(PCB* pcb){
	sys_free_mem(pcb);
	return 0;
}

PCB* find_pcb(char *process_name){
	
	int i;

	PCB *temp_process = ready_queue.head;	//points to the PCB at the head of the queue
	
	for(i = 0; i < ready_queue.count; i++){
		if(strcmp(process_name, temp_process->name) == 0){ // checks if strcmp returns 0 (0 means they match)
			return temp_process;
		}else {
			temp_process = temp_process -> next;	//moves to the next PCB in the linked list
		}
	}

	temp_process = blocked_queue.head;	//points to the PCB at the head of the queue
	
	for(i = 0; i < blocked_queue.count; i++){
		if(strcmp(process_name, temp_process->name) == 0){ // checks if strcmp returns 0 (0 means they match)
			return temp_process;
		}else {
			temp_process = temp_process -> next;	//moves to the next PCB in the linked list
		}
	}

	temp_process = suspended_ready_queue.head;	//points to the PCB at the head of the queue
	
	for(i = 0; i < suspended_ready_queue.count; i++){
		if(strcmp(process_name, temp_process->name) == 0){ // checks if strcmp returns 0 (0 means they match)
			return temp_process;
		}else {
			temp_process = temp_process -> next;	//moves to the next PCB in the linked list
		}
	}

	temp_process = suspended_blocked_queue.head;	//points to the PCB at the head of the queue
	
	for(i = 0; i < suspended_blocked_queue.count; i++){
		if(strcmp(process_name, temp_process->name) == 0){ // checks if strcmp returns 0 (0 means they match)
			return temp_process;
		}else {
			temp_process = temp_process -> next;	//moves to the next PCB in the linked list
		}
	}

	return NULL; // TODO ERROR CODE
}


int insert_pcb(PCB *pcb){

	if(pcb->suspended == 0){	//checks to make sure PCB is not suspended
		if(pcb->readystate == -1){ //If ready state is blocked (readystate of -1 means blocked)
			pcb->prev = blocked_queue.head;  //adds PCB to end of queue if blocked because blocked queue is FIFO
			blocked_queue.head = pcb;
			blocked_queue.count++;	// increment blocked_queue count
		}else if(pcb->readystate == 0){ //If ready state is ready (readystate of 0 means ready)
			int found = 0;
			PCB *temp_pcb = ready_queue.tail;	//set temp PCB to tail of ready_queue
			while(!found){	//for ready_queue, PCBs are inserted in order of priority 
				if(pcb->priority == temp_pcb->priority){	
					pcb->next = temp_pcb;	// There two lines insert the PCB into the linked list
					temp_pcb->prev->next = pcb;
					found = 1;
				}else if(temp_pcb->next == ready_queue.head){ //if the pcb is the highest priority, make it the new head of ready_queue
					ready_queue.head->next = pcb; 
					pcb->prev = ready_queue.head;
					ready_queue.head = pcb;
					found = 1;
				}else{
					temp_pcb = temp_pcb->next;	//moves onto next PCB in linked list
				}
			}
		}
	}else{						//If the PCB IS suspended, start here
		if(pcb->readystate == -1){ //If ready state is blocked (readystate of -1 means blocked)
			pcb->prev = blocked_queue.head;  //adds PCB to end of queue if blocked because blocked queue is FIFO
			blocked_queue.head = pcb;
			suspended_blocked_queue.count++;	// increment blocked_queue count
		}else if(pcb->readystate == 0){ //If ready state is ready (readystate of 0 means ready)
			int found = 0;
			PCB *temp_pcb = suspended_ready_queue.tail;	//set temp PCB to tail of ready_queue
			while(!found){	//for ready_queue, PCBs are inserted in order of priority 
				if(pcb->priority == temp_pcb->priority){	
					pcb->next = temp_pcb;	// There two lines insert the PCB into the linked list
					temp_pcb->prev->next = pcb;
					found = 1;
				}else if(temp_pcb->next == suspended_ready_queue.head){ //if the pcb is the highest priority, make it the new head of ready_queue
					suspended_ready_queue.head->next = pcb; 
					pcb->prev = suspended_ready_queue.head;
					suspended_ready_queue.head = pcb;
					found = 1;
				}else{
					temp_pcb = temp_pcb->next;	//moves onto next PCB in linked list
				}
			}
		}
	}
	return 0;
}

int remove_pcb(PCB* pcb){
	if(pcb->readystate == 0 && pcb->suspended == 0){	//ready
		PCB* temp_pcb = ready_queue.tail;
		int found = 0;
		while(!found){
			if(pcb == temp_pcb){
				temp_pcb->next->prev = temp_pcb->prev;
				temp_pcb->prev->next = temp_pcb->next;
			}else if(temp_pcb == ready_queue.head){
				return 0; // TODO ERROR CODE  PCB NOT FOUND
			}else{
				temp_pcb = temp_pcb->next;
			}
		}
	}
	else if(pcb->readystate == 0 && pcb->suspended == 1){ //suspended ready
		PCB* temp_pcb = suspended_ready_queue.tail;
		int found = 0;
		while(!found){
			if(pcb == temp_pcb){
				temp_pcb->next->prev = temp_pcb->prev;
				temp_pcb->prev->next = temp_pcb->next;
			}else if(temp_pcb == suspended_ready_queue.head){
				return 0; // TODO ERROR CODE  PCB NOT FOUND
			}else{
				temp_pcb = temp_pcb->next;
			}
		}
	}
	else if(pcb->readystate == -1 && pcb->suspended == 0){ //blocked
		PCB* temp_pcb = blocked_queue.tail;
		int found = 0;
		while(!found){
			if(pcb == temp_pcb){
				temp_pcb->next->prev = temp_pcb->prev;
				temp_pcb->prev->next = temp_pcb->next;
			}else if(temp_pcb == blocked_queue.head){
				return 0; // TODO ERROR CODE  PCB NOT FOUND
			}else{
				temp_pcb = temp_pcb->next;
			}
		}		
	}
	else if(pcb->readystate == -1 && pcb->suspended == 1){ //suspended blocked
		PCB* temp_pcb = suspended_blocked_queue.tail;
		int found = 0;
		while(!found){
			if(pcb == temp_pcb){
				temp_pcb->next->prev = temp_pcb->prev;
				temp_pcb->prev->next = temp_pcb->next;
			}else if(temp_pcb == suspended_blocked_queue.head){
				return 0; // TODO ERROR CODE  PCB NOT FOUND
			}else{
				temp_pcb = temp_pcb->next;
			}
		}		
	}	
	else{	// PROCESS NOT FOUND

		return 0; // TODO ERROR CODE
	}
	return 0;
}








