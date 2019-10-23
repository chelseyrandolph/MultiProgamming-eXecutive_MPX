
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

PCB* setup_pcb(char name[], int pclass, int priority){ // pclass refers to process class (system or user)

	PCB *new_pcb = allocate_pcb();		// allocates memory to the new PCB

	new_pcb -> top_of_stack = new_pcb -> bottom_of_stack + sizeof(new_pcb->stack) - sizeof(context);
	new_pcb -> context = (context*) new_pcb -> top_of_stack;

	if(strlen(name) >= 8){	// Process name must be at least 8 characters
		strcpy(new_pcb->name, name);		// sets the new process's name to name parameter
	}else{
		return NULL; 
	}
	if(priority >= 0 && priority <= 9){		// priority must be a number between 0 and 9
		new_pcb -> priority = priority;		// sets the new process's priority to priority parameter
	}else{
		return NULL; 
	}
	if(pclass == 0 || pclass ==1){			// process class must be either 0 for system, or 1 for user process
		new_pcb -> process_class = pclass;	// sets the new process's class to the pclass parameter
	}else{
		return NULL; 
	}
	new_pcb->readystate = 0; //ready
	new_pcb->suspended = 0;		//not suspended
	
	return new_pcb;
}

void remove_all(){
	PCB* pcb = ready_queue.head;
	while(pcb != NULL){
		delete_pcb(pcb->name);
		pcb = pcb->next;
	}
	pcb = suspended_ready_queue.head;
	while(pcb != NULL){
		delete_pcb(pcb->name);
		pcb = pcb->next;
	}
	pcb = blocked_queue.head;
	while(pcb != NULL){
		delete_pcb(pcb->name);
		pcb = pcb->next;
	}
	pcb = suspended_blocked_queue.head;
	while(pcb != NULL){
		delete_pcb(pcb->name);
		pcb = pcb->next;
	}
}

int free_pcb(PCB* pcb){
	sys_free_mem(pcb->bottom_of_stack); 
	sys_free_mem(pcb);
	return 0;
}

PCB* find_pcb(char process_name[]){
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
	found = 0;
	while(!found){
		if(strcmp(temp_process->name, process_name) == 0){
			return temp_process;
		}else if(temp_process == blocked_queue.tail){
			found = 1;
		}else{
			temp_process = temp_process->next;
		}
	}

	temp_process = suspended_ready_queue.head;	//points to the PCB at the head of the queue
	found = 0;
	while(!found){
		if(strcmp(temp_process->name, process_name) == 0){
			return temp_process;
		}else if(temp_process == suspended_ready_queue.tail){
			found = 1;
		}else{
			temp_process = temp_process->next;
		}
	}

	temp_process = suspended_blocked_queue.head;	//points to the PCB at the head of the queue
	found = 0;
	while(!found){
		if(strcmp(temp_process->name, process_name) == 0){
			return temp_process;
		}else if(temp_process == suspended_blocked_queue.tail){
			found = 1;
		}else{
			temp_process = temp_process->next;
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
				write_text_red("Process Not Found... exiting\n");
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
				write_text_red("Process Not Found... exiting\n");
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
				write_text_red("Process Not Found... exiting\n");
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
				write_text_red("Process Not Found... exiting\n");
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
	PCB *temp = find_pcb(name);
	if(temp != NULL){
		write_text_bold_yellow("There is already a process with that name\n");
		return 0;
	}
	if(strlen(name) < 8){	// Process name must be at least 8 characters
		write_text_bold_red("INVALID NAME... exiting\n\n\n");
		return NULL; 
	}
	if(priority < 0 || priority > 9){		// priority must be a number between 0 and 9
		write_text_bold_red("INVALID PRIORITY... exiting\n\n\n");
		return NULL;	
	}
	if(pclass != 0 && pclass !=1){		// process class must be either 0 for system, or 1 for user process
		write_text_bold_red("INVALID CLASS... exiting\n\n\n");
		return NULL; 	
	}
	insert_pcb(setup_pcb(name, pclass, priority)); // Insert a setup PCB (insert_pcb() takes are of placing it in the appropriate queue
	return 0;
}


int delete_pcb(char name[]){
	PCB *pcb = find_pcb(name);
	remove_pcb(pcb);
	free_pcb(pcb);
	return 0;
}

int block_pcb(char name[]){
	PCB *pcb = find_pcb(name);
	if(pcb == NULL){
		write_text_bold_red("Process Not Found\n");
	}
	remove_pcb(pcb);		//takes the pcb out of the queue its in
	pcb->readystate = -1;	//sets readystate = blocked
	insert_pcb(pcb);			//inserts it into the blocked queue
	return 0;
}

int unblock_pcb(char name[]){
	PCB *pcb = find_pcb(name);
	if(pcb == NULL){
		write_text_bold_red("Process Not Found\n");
	}
	remove_pcb(pcb);		//takes the pcb out of the queue its in
	pcb->readystate = 0;	//sets readystate = ready
	insert_pcb(pcb);			//inserts it into the ready (or suspended_ready) queue
	return 0;
}

int suspend_pcb(char name[]){
	PCB *pcb = find_pcb(name);
	remove_pcb(pcb);		//takes the pcb out of the queue its in
	pcb->suspended = 1;	//sets suspended = suspended
	insert_pcb(pcb);		//inserts it into the appropriate suspended queue

	return 0;
}

int resume_pcb(char name[]){
	PCB *pcb = find_pcb(name);

	remove_pcb(pcb);		//takes the pcb out of the queue its in
	pcb->suspended = 0;	//sets suspended = unsuspended
	insert_pcb(pcb);		//inserts it into the appropriate suspended queue
	return 0;
}

int set_pcb_priority(char name[], int new_priority){
	PCB *pcb = find_pcb(name);
	remove_pcb(pcb);
	pcb->priority = new_priority;
	insert_pcb(pcb);
	return 0;
}

int show_pcb(char name[]){
	PCB *pcb = find_pcb(name);
	if(pcb == NULL){
		write_text_yellow("Process not found\n\n");
	}else{
		char namestr[17];
		strcpy(namestr, name);
		int i;
		for(i = strlen(name); i < 16; i++){	
			namestr[i] = ' ';
		}
		namestr[16] = '\0';
		write_text(namestr);
		write_text(" |");
		if(pcb->process_class == 1){
			write_text_blue("    User Process       ");
		}else if(pcb->process_class == 0){
			write_text_blue("    System Process     ");
		}else{
			return 0;
		}
		write_text("|");
		if(pcb->readystate == 1){
			write_text("     Running     ");
		}else if(pcb->readystate == 0){
			write_text_green("      Ready      ");
		}else if(pcb->readystate == -1){
			write_text_red("     Blocked     ");
		}else{
			return 0;
		}
		write_text("|");
		if(pcb->suspended == 1){
			write_text_bold_red("       SUSPENDED		   ");
		}else if(pcb->suspended == 0){
			write_text("        not suspended	   ");
		}else{
			return 0; 
		}
		write_text("| ");
		write_text(itoa(pcb->priority));
		write_text("\n");
	}
	return 1;
}

int show_ready(){
	int done = 0;
	PCB *pcb = ready_queue.head;
	if(pcb != NULL){	
		write_text_bold_magenta("\n\nReady Processes:\n\n");
		write_text_bold_blue("     Name              Process Type         Ready/Blocked     Suspended/Not Suspended      Priority\n\n");
		write_text_bold_blue("----------------------------------------------------------------------------------------------------\n\n");
		while(!done){
			show_pcb(pcb->name);
			write_text("\n");
			if(pcb == ready_queue.tail){
				done = 1;
			}else {
				pcb = pcb->next;
			}
		}
	}
	
	done = 0;
	pcb = suspended_ready_queue.head;
	if(pcb != NULL){
		write_text_bold_magenta("\n\nSuspended-ready Processes:\n\n");
		write_text_bold_blue("     Name              Process Type         Ready/Blocked     Suspended/Not Suspended      Priority\n\n");
		write_text_bold_blue("----------------------------------------------------------------------------------------------------\n\n");
		while(!done){
			show_pcb(pcb->name);
			write_text("\n");
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
	int done = 0;
	PCB *pcb = blocked_queue.head;
	if(pcb != NULL){
		write_text_bold_magenta("\n\nBlocked Processes:\n\n");
		write_text_bold_blue("     Name              Process Type         Ready/Blocked     Suspended/Not Suspended      Priority\n\n");
		write_text_bold_blue("----------------------------------------------------------------------------------------------------\n");
		while(!done){
			show_pcb(pcb->name);
			write_text("\n");
			if(pcb == blocked_queue.tail){
				done = 1;
			}else {
				pcb = pcb->next;
			}
		}	
	}
	
	done = 0;
	pcb = suspended_blocked_queue.head;
		if(pcb != NULL){
		write_text_bold_magenta("\n\nSuspended-blocked Processes:\n\n");
		write_text_bold_blue("     Name              Process Type         Ready/Blocked     Suspended/Not Suspended      Priority\n\n");
		write_text_bold_blue("----------------------------------------------------------------------------------------------------\n\n");
		while(!done){
			show_pcb(pcb->name);
			write_text("\n\n");
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
	write_text("\n\n");
	return 0;
}

