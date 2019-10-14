#include <string.h>
#include <system.h>
#include <core/io.h>
#include <core/serial.h>
#include <core/tables.h>
#include <core/interrupts.h>
#include "pcb.h" 
#include "../mpx_supt.h"
#include "../R1/time.h"
#include "../../lib/colortext.h"
#include "../R1/comhand.h"
//#include "../R2/pcb.h"

// create a global cop, and context switch that represent to the context in struct
PCB* cop = NULL;
context* contextSwitch = NULL;
extern param params;
extern queue ready_queue;

u32int* sys_call(context*registers){
// create a temporary pcn the reperesent to the head of ready_queue
PCB* temporary_ready = ready_queue.head;
// check if the cop not equall null , for Idle let the pcb implies to the top of the stack then 
// remove the cop from ready queue and insert it in the cop
if(cop !=NULL){
if(params.op_code == IDLE){
cop-> top_of_stack = (unsigned char*)registers;
remove_pcb(cop);
 insert_pcb(cop); 
}
// if the params is equal to EXIt , remove the cop of pcb and free it.
else if(params.op_code == EXIT){
remove_pcb(cop);
free_pcb(cop);
}
}
// if cop is not null , then let the context swithc equal to the regesters
else {
contextSwitch = registers;} 
// cheacking if the temporay of the raady queue is not null, the checking if we are in the head 
// of ready queue , then the valuse of cop is equal to the ready_queue.head, otherwise add it the the tail.
if (temporary_ready !=NULL){
if(ready_queue.head){
cop = ready_queue.head;
// return the function &cop to the to of the stack
return (u32int*) &cop -> top_of_stack;
}
else{
temporary_ready = temporary_ready->next;
}
}
// otherwise return the function & the context switch
return (u32int*) & contextSwitch;
}
void Yield(){
asm volatile("int $60");
}
