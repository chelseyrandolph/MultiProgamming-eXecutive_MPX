#include <string.h>
#include <system.h>
#include <core/io.h>
#include <core/serial.h>
#include <core/tables.h>
#include <core/interrupts.h>
#include "../mpx_supt.h"
#include "../R1/time.h"
#include "../../lib/colortext.h"
#include "../R1/comhand.h"
#include "Test_process.h"
#include "pcb.h"

// create a global cop, and context switch that represent to the context in struct
PCB* cop = NULL;
context* contextSwitch = NULL;
extern param params;
extern queue ready_queue;


u32int* sys_call(context* registers){
// create a temporary pcb that reperesents to the head of ready_queue	
	PCB* temporary_ready = ready_queue.head;

	if(cop == NULL){
		contextSwitch = registers;
	}

	if(params.op_code == IDLE && cop != NULL){
		cop -> top_of_stack = (unsigned char*) registers -> esp;
		cop -> context = registers;
		insert_pcb(cop);
	}else if(params.op_code == EXIT){
		cop = NULL;
	}

	if(temporary_ready != NULL){
		cop = temporary_ready;
		remove_pcb(cop);
	}else{
		cop = NULL;
		return (u32int*) contextSwitch;
	}
	return (u32int*) cop -> context;
}


void yield(){
asm volatile("int $60");
}


void loadr3(){
	int i = 1;
	//Looping through each test process
	for(i = 1; i <= 5; i++){
		char name[9] = "process";
		strcat(name, itoa(i));
	//	int nameSize = sizeof(name);
		create_pcb(name, 1, 1);
		PCB* newPCB = find_pcb(name);
		suspend_pcb(name);
		context* cp = (context*)(newPCB->top_of_stack);
		memset(cp,0,sizeof(context));
		cp->fs = 0x10;
		cp->gs = 0x10;
		cp->ds = 0x10;
		cp->es = 0x10;
		cp->cs = 0x8;
		cp->ebp = (u32int)(newPCB->bottom_of_stack);
		cp->esp = (u32int)(newPCB->top_of_stack);

		switch(i){
		case 1:				//include processes file
			
			cp->eip = (u32int) proc1;
			break;
		case 2:
			
			cp->eip = (u32int) proc2;
			break;
		case 3:
			
			cp->eip = (u32int) proc3;
			break;
		case 4:
			
			cp->eip = (u32int) proc4;
			break;
		case 5:
			
			cp->eip = (u32int) proc5;
			break;
		}
		cp->eflags = 0x202;
	}
}




	
