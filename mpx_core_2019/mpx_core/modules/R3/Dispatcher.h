#include "pcb.h"

#ifndef Modual_R3_Dispatch
#define Modual-R3_Dispatch
context* context;
u32int* sys_call(context* registers);
void yield();
void loadr3();
#endif

