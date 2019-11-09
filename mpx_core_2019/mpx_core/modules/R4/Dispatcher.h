#include "pcb.h"

/*
sys_call performs the context switch for the current operating process. This is called from the assembly code in irq.s .
Return Type: u32int*
Parameters: context*: value from the context struct
*/
u32int* sys_call(context* registers);


/*
yield calls an interrupt 
Return Type: void
Parameters: none
*/
void yield();


/*
loadr3 load the given test processes
Return Type: void
Parameters: none
*/
void loadr3();


/*
loadprocesses loads the given test processes
Return Type: void
Parameters: none
*/
void loadProcess();


/*
infinite prints out a message everytime it runs. It should have a high priority.  
Return Type: void
Parameters: none
*/
void infinite();


/*
sys_load_proc loads any process into the ready queue.
Return Type: void
Parameters: none
*/
void sys_load_proc();

