
#include <string.h>

#include "pcb.h" 
#include "../mpx_supt.h"


queue ready_queue = {.head = NULL, .tail = NULL, .count = 0};
queue blocked_queue = {.head = NULL, .tail = NULL, .count = 0};
queue suspended_ready_queue = {.head = NULL, .tail = NULL, .count = 0};
queue suspended_blocked_queue = {.head = NULL, .tail = NULL, .count = 0};

PCB* allocate_pcb(){
	
	PCB *pcb = sys_alloc_mem(sizeof(pcb));
	
	return pcb;
}

PCB* setup_pcb(char *name, int pclass, int priority){

	PCB *new_pcb = allocate_pcb();		// allocates memory to the new PCB

	strcpy(new_pcb -> name , name);		// sets the new process's name to name parameter
	new_pcb -> priority = priority;		// sets the new process's priority to priority parameter
	new_pcb -> process_class = pclass;	// sets the new process's class to the pclass parameter
	
	/*
		TODO - check each param 
		* pclass either 0 or 1
		* name is a valid str with length AT LEAST 8
		* priority between 0 and 9
		* return ERROR MESSAGES
	*/
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








