#include "pcb.h"

#ifndef Module_R3_Dispatch
#define Module_R3_Dispatch
//context* context;
u32int* sys_call(context* registers);
void yield();
void loadr3();
#endif

