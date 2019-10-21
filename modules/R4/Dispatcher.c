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
	}else{

		if(params.op_code == IDLE){
			cop -> top_of_stack = (unsigned char*) registers;
			cop -> context = registers;
		
		}else if(params.op_code == EXIT){
			free_pcb(cop);
		}
	}

	if(temporary_ready != NULL){
		remove_pcb(temporary_ready);
		//New processes will get added back
		if(cop != NULL && params.op_code != EXIT){
			insert_pcb(cop);
		}
		cop = temporary_ready;
		return (u32int*)cop -> top_of_stack;
	}else{
		free_pcb(cop);
		cop = NULL;
		return (u32int*) contextSwitch;
	}
	return (u32int*) cop -> context;
}


void yield(){
asm volatile("int $60");
}



void infinite(){
	char *infinite = "INFINITE PROCESS IS EXECUTING.\n";
	int infiniteSize = sizeof(infinite);
	
	while(1){
		sys_req(WRITE, DEFAULT_DEVICE, infinite, &infiniteSize);
		sys_req(IDLE, COM1, NULL,NULL);
	}
}

void loadProcess(char name[], int class, int priority, void* function){

	PCB* new_pcb = setup_pcb(name, class, priority);
	insert_pcb(new_pcb);
	context* cp = (context*)(new_pcb -> top_of_stack);
	memset(cp, 0, sizeof(context));		
	cp->fs = 0x10;
	cp->gs = 0x10;
	cp->ds = 0x10;
	cp->es = 0x10;
	cp->cs = 0x8;
	cp->ebp = (u32int)(new_pcb->bottom_of_stack);
	cp->esp = (u32int)(new_pcb->top_of_stack);
	cp->eip = (u32int) function;
}

void loadr3(){
	loadProcess("process01", 1, 1, &proc1);
	suspend_pcb("process01");
	loadProcess("process02", 1, 1, &proc2);
	suspend_pcb("process02");
	loadProcess("process03", 1, 1, &proc3);
	suspend_pcb("process03");
	loadProcess("process04", 1, 1, &proc4);
	suspend_pcb("process04");
	loadProcess("process05", 1, 1, &proc5);
	suspend_pcb("process05");

}




