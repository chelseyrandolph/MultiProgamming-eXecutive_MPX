
#include <string.h>
#include <system.h>
#include <core/io.h>
#include <core/serial.h>
#include "pcb.h" 
#include "../mpx_supt.h"
#include "../R1/time.h"
#include "../../lib/colortext.h"


queue ready_queue = {.head = NULL, .tail = NULL, .count = 0};
queue blocked_queue = {.head = NULL, .tail = NULL, .count = 0};
queue suspended_ready_queue = {.head = NULL, .tail = NULL, .count = 0};
queue suspended_blocked_queue = {.head = NULL, .tail = NULL, .count = 0};



PCB* allocate_pcb(){
	
	PCB *pcb = sys_alloc_mem(sizeof(pcb));
	pcb -> bottom_of_stack = sys_alloc_mem(1024);
	return pcb;
}

void remove_all(){

	PCB* pcb = ready_queue.head;
	while(pcb != NULL){
		delete_pcb(pcb->name);
		pcb = pcb -> next;
	}
	pcb = suspended_ready_queue.head;
	while(pcb != NULL){
		delete_pcb(pcb->name);
		pcb = pcb -> next;
	}
	pcb = blocked_queue.head;
	while(pcb != NULL){
		delete_pcb(pcb->name);
		pcb = pcb -> next;
	}
	pcb = suspended_blocked_queue.head;
	while(pcb != NULL){
		delete_pcb(pcb->name);
		pcb = pcb -> next;
	}

}

PCB* setup_pcb(char name[], int pclass, int priority){ // pclass refers to process class (system or user)

	PCB *new_pcb = allocate_pcb();		// allocates memory to the new PCB

	new_pcb -> top_of_stack = new_pcb -> bottom_of_stack + sizeof(new_pcb->stack) - sizeof(context);
	new_pcb -> context = (context*) new_pcb -> top_of_stack;

	if(strlen(name) >= 8){	// Process name must be at least 8 characters
		strcpy(new_pcb -> name , name);		// sets the new process's name to name parameter
	}else{
		return NULL; 
	}
	if(priority >= 0 && priority <= 9){		// priority must be a number between 0 and 9
		new_pcb -> priority = priority;		// sets the new process's priority to priority parameter
	}else{
		return NULL; 
	}
	//pclass = 1; //HARD-CODED
	if(pclass == 0 || pclass ==1){			// process class must be either 0 for system, or 1 for user process
		new_pcb -> process_class = pclass;	// sets the new process's class to the pclass parameter
	}else{
		return NULL; 
	}
	new_pcb->readystate = 0; //ready
	new_pcb->suspended = 0;		//not suspended
	
	return new_pcb;
}

int free_pcb(PCB* pcb){
	sys_free_mem(pcb->bottom_of_stack); 
	sys_free_mem(pcb);
	return 0;
}

PCB* find_pcb(char *process_name){
	
	int i;

	PCB *temp_process = ready_queue.head;	//points to the PCB at the head of the queue
	
	int found = 0;
	while(!found){
		if(strcmp(temp_process->name, process_name) == 0){
			return temp_process;
		}else if(temp_process == ready_queue.tail){
			found = 1;
		}else{
			temp_process = temp_process->next;
		}
	}

	temp_process = blocked_queue.head;	//points to the PCB at the head of the queue
	
	for(i = 0; i < blocked_queue.count; i++){
		if(strcmp(process_name, temp_process->name) == 0){ // checks if strcmp returns 0 (0 means they match)
			return temp_process;
		}else {
			temp_process = temp_process->next;	//moves to the next PCB in the linked list
		}
	}

	temp_process = suspended_ready_queue.head;	//points to the PCB at the head of the queue
	
	for(i = 0; i < suspended_ready_queue.count; i++){
		if(strcmp(process_name, temp_process->name) == 0){ // checks if strcmp returns 0 (0 means they match)
			return temp_process;
		}else {
			temp_process = temp_process->next;	//moves to the next PCB in the linked list
		}
	}

	temp_process = suspended_blocked_queue.head;	//points to the PCB at the head of the queue
	
	for(i = 0; i < suspended_blocked_queue.count; i++){
		if(strcmp(process_name, temp_process->name) == 0){ // checks if strcmp returns 0 (0 means they match)
			return temp_process;
		}else {
			temp_process = temp_process->next;	//moves to the next PCB in the linked list
		}
	}

	return NULL;
}


int insert_pcb(PCB *pcb){
	pcb->next = NULL;
	pcb->prev = NULL;
	if(pcb->readystate == 0 && pcb->suspended == 0){	//ready_queue
		if(ready_queue.count == 0){
			ready_queue.head = pcb;
			ready_queue.tail = pcb;
			ready_queue.count++;
		}else{
			int found = 0;
			PCB *temp_pcb = ready_queue.head;
			while(!found){
				
				if(temp_pcb->priority > pcb->priority){
					pcb->next = temp_pcb;
					temp_pcb->prev->next = pcb;
					pcb->prev = temp_pcb->prev;
					temp_pcb->prev = pcb;
					found = 1;
					if(pcb->prev == NULL){
						ready_queue.head = pcb;
					}
					ready_queue.count++;
				}else if(temp_pcb == ready_queue.tail){
					ready_queue.tail = pcb;
					temp_pcb->next = pcb;
					pcb->prev = temp_pcb;
					found = 1;
					ready_queue.count++;
				}else{
					temp_pcb = temp_pcb->next;

				}
			}

		}
	}
	if(pcb->readystate == 0 && pcb->suspended == 1){	//suspended_ready_queue
		if(suspended_ready_queue.count == 0){
			suspended_ready_queue.head = pcb;
			suspended_ready_queue.tail = pcb;
			suspended_ready_queue.count++;
		}else{
			int found = 0;
			PCB *temp_pcb = suspended_ready_queue.head;
			while(!found){
				
				if(temp_pcb->priority > pcb->priority){
					pcb->next = temp_pcb;
					temp_pcb->prev->next = pcb;
					pcb->prev = temp_pcb->prev;
					temp_pcb->prev = pcb;
					found = 1;
					if(pcb->prev == NULL){
						suspended_ready_queue.head = pcb;
					}
					suspended_ready_queue.count++;
				}else if(temp_pcb == suspended_ready_queue.tail){
					suspended_ready_queue.tail = pcb;
					temp_pcb->next = pcb;
					pcb->prev = temp_pcb;
					found = 1;
					suspended_ready_queue.count++;
				}else{
					temp_pcb = temp_pcb->next;
					
				}
			}

		}
	}
	if(pcb->readystate == -1 && pcb->suspended == 0){	//blocked_queue
		if(blocked_queue.count == 0){
			blocked_queue.head = pcb;
			blocked_queue.tail = pcb;
			blocked_queue.count++;
		}else{
			PCB *temp_pcb = blocked_queue.head;
			temp_pcb->prev = pcb;
			pcb->next = temp_pcb;
			blocked_queue.head = pcb;
			blocked_queue.count++;
		}
	}
	if(pcb->readystate == -1 && pcb->suspended == 1){	//suspended_blocked_queue
		
		if(suspended_blocked_queue.count == 0){
			suspended_blocked_queue.head = pcb;
			suspended_blocked_queue.tail = pcb;
			suspended_blocked_queue.count++;
		}else{
			PCB *temp_pcb = ready_queue.head;
			temp_pcb->prev = pcb;
			pcb->next = temp_pcb;
			suspended_blocked_queue.head = pcb;
			suspended_blocked_queue.count++;
		}
	}
	return 0;
}

int remove_pcb(PCB* pcb){
	int error_msg_size = 50; // use this for sys_req(Write)
	if(pcb->readystate == 0 && pcb->suspended == 0){ // ready_queue
		PCB *temp_pcb = ready_queue.head;
		int found = 0;
		while(!found){
			if(temp_pcb == pcb && temp_pcb != NULL){		//remove from queue and re-link its neighbors
				if(pcb == ready_queue.head){		// reassigns head if needed
					ready_queue.head = pcb->next;
				}
				if(pcb == ready_queue.tail){		// reassign tail if needed
					ready_queue.tail = pcb->prev;
				}
				pcb->next->prev = pcb->prev;
				pcb->prev->next = pcb->next;
				ready_queue.count--;
				found = 1;		
			}else if(temp_pcb == ready_queue.tail && temp_pcb !=pcb){
				sys_req(WRITE, DEFAULT_DEVICE, "\033[0;31mProcess Not Found... exitting\033[0m", &error_msg_size);
				return NULL;
			}else{
				temp_pcb = temp_pcb->next;
			}
		}
	}
	if(pcb->readystate == 0 && pcb->suspended == 1){ // suspended_ready_queue
		PCB *temp_pcb = suspended_ready_queue.head;
		int found = 0;
		while(!found && temp_pcb != NULL){
			if(temp_pcb == pcb){		//remove from queue and re-link its neighbors
				if(pcb == suspended_ready_queue.head){		// reassigns head if needed
					suspended_ready_queue.head = pcb->next;
				}
				if(pcb == suspended_ready_queue.tail){		// reassign tail if needed
					suspended_ready_queue.tail = pcb->prev;
				}
				pcb->next->prev = pcb->prev;
				pcb->prev->next = pcb->next;	
				suspended_ready_queue.count--;	
				found = 1;
			}else if(temp_pcb == suspended_ready_queue.tail && temp_pcb !=pcb){
				sys_req(WRITE, DEFAULT_DEVICE, "\033[0;31mProcess Not Found... exitting\033[0m", &error_msg_size);
				return NULL;
			}else{
				temp_pcb = temp_pcb->next;
			}
		}
	}
	if(pcb->readystate == -1 && pcb->suspended == 0){ // blocked_queue
		PCB *temp_pcb = blocked_queue.head;
		int found = 0;
		while(!found && temp_pcb != NULL){
			if(blocked_queue.count == 1){
				blocked_queue.head = NULL;
				blocked_queue.tail = NULL;
				blocked_queue.count = 0;
				return 0;
			}
			if(temp_pcb == pcb){		//remove from queue and re-link its neighbors
				if(pcb == blocked_queue.head){		// reassigns head if needed
					blocked_queue.head = pcb->next;
				}
				if(pcb == blocked_queue.tail){		// reassign tail if needed
					blocked_queue.tail = pcb->prev;
				}
				pcb->next->prev = pcb->prev;
				pcb->prev->next = pcb->next;		
				blocked_queue.count--;
				found = 1;
			}else if(temp_pcb == blocked_queue.tail && temp_pcb !=pcb){
				sys_req(WRITE, DEFAULT_DEVICE, "\033[0;31mProcess Not Found... exitting\033[0m", &error_msg_size);
				return NULL;
			}else{
				temp_pcb = temp_pcb->next;
			}
		}
	}
	if(pcb->readystate == -1 && pcb->suspended == 1){ // suspended_blocked_queue
		PCB *temp_pcb = suspended_blocked_queue.head;
		int found = 0;
		while(!found){
			if(temp_pcb == pcb){		//remove from queue and re-link its neighbors
				if(pcb == suspended_blocked_queue.head){		// reassigns head if needed
					suspended_blocked_queue.head = pcb->next;
				}
				if(pcb == suspended_blocked_queue.tail){		// reassign tail if needed
					suspended_blocked_queue.tail = pcb->prev;
				}
				pcb->next->prev = pcb->prev;
				pcb->prev->next = pcb->next;		
				suspended_blocked_queue.count--;
				found = 1;
			}else if(temp_pcb == suspended_blocked_queue.tail && temp_pcb !=pcb){
				sys_req(WRITE, DEFAULT_DEVICE, "\033[0;31mProcess Not Found... exitting\033[0m", &error_msg_size);
				return NULL;
			}else{
				temp_pcb = temp_pcb->next;
			}
		}
	}
	return 0;
}

/*========================= USER COMMANDS ========================*/

int create_pcb(char name[], int pclass, int priority){ 
	int msgSize = 75;
	PCB *temp = find_pcb(name);
	if(temp != NULL){
		write_text_bold_yellow("There is already a process with that name\n");
		return 0;
	}
	if(strlen(name) < 8){	// Process name must be at least 8 characters
		char invalid_name_msg[75] = "\033[1;31mINVALID NAME... exiting\033[0m\n\n";
		sys_req(WRITE, DEFAULT_DEVICE, invalid_name_msg, &msgSize);
		return NULL; 
	}
	if(priority < 0 || priority > 9){		// priority must be a number between 0 and 9
		char invalid_name_msg[75] = "\033[1;31mINVALID PRIORITY... exiting\033[0m\n\n";
		sys_req(WRITE, DEFAULT_DEVICE, invalid_name_msg, &msgSize);
		return NULL;	
	}
	if(pclass != 0 && pclass !=1){		// process class must be either 0 for system, or 1 for user process
		char invalid_name_msg[75] = "\033[1;31mINVALID CLASS... exiting\033[0m\n\n";
		sys_req(WRITE, DEFAULT_DEVICE, invalid_name_msg, &msgSize);
		return NULL; 	
	}
	insert_pcb(setup_pcb(name, pclass, priority)); // Insert a setup PCB (insert_pcb() takes are of placing it in the appropriate queue
	return 0;
}

/*
If NOT suspended, the process can NOT be deleted
If suspended, it may be deleted
***INFINITE PROCESS***
*/
int delete_pcb(char *name){
	int msgSize = 75;
	PCB *pcb = find_pcb(name);
	//suspended, deleted pcb
	if(strcmp(pcb->name,"infinite_process") == 0 && pcb->suspended == 1){
		remove_pcb(pcb);
		free_pcb(pcb);
	//not suspended, does not delete
	}else if(strcmp(pcb->name,"infinite_process") == 0 && pcb->suspended == 0){
		char errMsg[75] = "You must suspend the infinite process before deleting it.\n";
		sys_req(WRITE, DEFAULT_DEVICE, errMsg, &msgSize);
		return 0;
	}else{
		remove_pcb(pcb);
		free_pcb(pcb);
	}
	return 0;
}

int block_pcb(char *name){
	PCB *pcb = find_pcb(name);
	if(pcb == NULL){
		write_text_bold_red("Process Not Found\n");
	}
	remove_pcb(pcb);		//takes the pcb out of the queue its in
	pcb->readystate = -1;	//sets readystate = blocked
	insert_pcb(pcb);			//inserts it into the blocked queue
	return 0;
}

int unblock_pcb(char *name){
	PCB *pcb = find_pcb(name);
	if(pcb == NULL){
		write_text_bold_red("Process Not Found\n");
	}
	remove_pcb(pcb);		//takes the pcb out of the queue its in
	pcb->readystate = 0;	//sets readystate = ready
	insert_pcb(pcb);			//inserts it into the ready (or suspended_ready) queue
	return 0;
}

int suspend_pcb(char *name){
	PCB *pcb = find_pcb(name);
	remove_pcb(pcb);		//takes the pcb out of the queue its in
	pcb->suspended = 1;	//sets suspended = suspended
	insert_pcb(pcb);		//inserts it into the appropriate suspended queue

	return 0;
}

int resume_pcb(char *name){
	PCB *pcb = find_pcb(name);

	remove_pcb(pcb);		//takes the pcb out of the queue its in
	pcb->suspended = 0;	//sets suspended = unsuspended
	insert_pcb(pcb);		//inserts it into the appropriate suspended queue
	return 0;
}

int set_pcb_priority(char *name, int new_priority){
	PCB *pcb = find_pcb(name);
	remove_pcb(pcb);
	pcb->priority = new_priority;
	insert_pcb(pcb);
	return 0;
}

int show_pcb(char *name){
	int txtSize = 50;
	int name_size = 16;
	PCB *pcb = find_pcb(name);
	char *pclass;
	char *readystate_str;
	char *suspended_str;
	char vertline[50] = " |";
	char newline[50] = " \n";
	char namestr[17];
	strcpy(namestr, name);
	int i;
	for(i = strlen(name); i < name_size; i++){
		
		namestr[i] = ' ';
	}
	namestr[16] = '\0';
	
	if(pcb->process_class == 1){
		pclass = " \033[0;34m    User Process    \033[0m   |";
	}else if(pcb->process_class == 0){
		pclass = " \033[0;36m    System Process    \033[0m |";
	}else{
		return 0;
	}
	if(pcb->readystate == 1){
		readystate_str = "     Running     |";
	}else if(pcb->readystate == 0){
		readystate_str = " \033[0;32m     Ready     \033[0m |";
	}else if(pcb->readystate == -1){
		readystate_str = " \033[0;31m    Blocked    \033[0m |";
	}else{
		return 0;
	}
	if(pcb->suspended == 1){
		suspended_str = " \033[1;31m       SUSPENDED       \033[0m     | ";
	}else if(pcb->suspended == 0){
		suspended_str = "        not suspended        | ";
	}else{
		return 0; 
	}

	sys_req(WRITE, DEFAULT_DEVICE, namestr, &txtSize);
	sys_req(WRITE, DEFAULT_DEVICE, vertline, &txtSize);
	sys_req(WRITE, DEFAULT_DEVICE, pclass, &name_size);
	sys_req(WRITE, DEFAULT_DEVICE, readystate_str, &name_size);
	sys_req(WRITE, DEFAULT_DEVICE, suspended_str, &name_size);
	sys_req(WRITE, DEFAULT_DEVICE, itoa(pcb->priority), &txtSize);
	sys_req(WRITE, DEFAULT_DEVICE, newline, &txtSize);
	return 1;
}

int show_ready(){
	int done = 0;
	int txtSize = 200;
	PCB *pcb = ready_queue.head;
	char process_msg[200] = "\033[1;34m     Name              Process Type         Ready/Blocked     Suspended/Not Suspended      Priority\033[0m\n";
	char lines[200] = "\033[1;34m----------------------------------------------------------------------------------------------------\033[0m\n";
	char newline[200] = "\n";
	char ready_msg[200] = "\033[1;35mReady Processes:\033[0m\n\n";

	if(ready_queue.head != NULL){
		
		sys_req(WRITE, DEFAULT_DEVICE, ready_msg, &txtSize);

		
		sys_req(WRITE, DEFAULT_DEVICE, process_msg, &txtSize);
		sys_req(WRITE, DEFAULT_DEVICE, lines, &txtSize);
		
		while(!done   && pcb !=NULL){
			show_pcb(pcb->name);
			sys_req(WRITE, DEFAULT_DEVICE, newline, &txtSize);
	
			if(pcb == ready_queue.tail){
				done = 1;
			}else {
				pcb = pcb->next;
			}
		}
	}
	
	done = 0;
	char sready_msg[200] = "\033[1;35mSuspended-ready Processes:\033[0m\n\n";

	if(suspended_ready_queue.head != NULL){
		sys_req(WRITE, DEFAULT_DEVICE, sready_msg, &txtSize);
		pcb = suspended_ready_queue.head;
		sys_req(WRITE, DEFAULT_DEVICE, process_msg, &txtSize);
		sys_req(WRITE, DEFAULT_DEVICE, lines, &txtSize);
		while(!done && pcb != NULL){
			show_pcb(pcb->name);
			sys_req(WRITE, DEFAULT_DEVICE, newline, &txtSize);
			if(pcb == suspended_ready_queue.tail){
				done = 1;
			}else {
				pcb = pcb->next;
			}
		}
	}
	return 0;
}

int show_blocked(){
	int txtSize = 200;
	char process_msg[200] = "\033[1;34m     Name              Process Type         Ready/Blocked     Suspended/Not Suspended      Priority\033[0m\n";
	char lines[200] = "\033[1;34m----------------------------------------------------------------------------------------------------\033[0m\n";
	char newline[200] = "\n";
	char blocked_msg[200] = "\033[1;35mBlocked Processes:\033[0m\n\n";
	int done = 0;
	PCB *pcb = blocked_queue.head;

	if(blocked_queue.head != NULL){
		sys_req(WRITE, DEFAULT_DEVICE, blocked_msg, &txtSize);
		sys_req(WRITE, DEFAULT_DEVICE, process_msg, &txtSize);
		sys_req(WRITE, DEFAULT_DEVICE, lines, &txtSize);
		while(!done && pcb != NULL){
			show_pcb(pcb->name);
			sys_req(WRITE, DEFAULT_DEVICE, newline, &txtSize);
			if(pcb == blocked_queue.tail){
				done = 1;
			}else {
				pcb = pcb->next;
			}
		}	
	}
	
	done = 0;
	char sblocked_msg[200] = "\033[1;35mSuspended-blocked Processes:\033[0m\n\n";

	if(suspended_blocked_queue.head != NULL){
		sys_req(WRITE, DEFAULT_DEVICE, sblocked_msg, &txtSize);
		pcb = suspended_blocked_queue.head;
		sys_req(WRITE, DEFAULT_DEVICE, process_msg, &txtSize);
		sys_req(WRITE, DEFAULT_DEVICE, lines, &txtSize);
		while(!done && pcb != NULL){
			show_pcb(pcb->name);
			sys_req(WRITE, DEFAULT_DEVICE, newline, &txtSize);
			if(pcb == suspended_blocked_queue.tail){
				done = 1;
			}else {
				pcb = pcb->next;
			}
		}
	}	
	return 0;
}

int show_all(){
	show_ready();
	show_blocked();
	int nl = 2;
	sys_req(WRITE, DEFAULT_DEVICE, "\n", &nl);
	return 0;
}



