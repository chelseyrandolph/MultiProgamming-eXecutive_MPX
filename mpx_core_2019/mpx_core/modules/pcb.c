
typedef struct PCB {
	char name[30];
	int class;		// 0 => system process  |  1 => user process
	int priority;
	int readystate;  // -1 => blocked  |  0 => ready   |  1 => blocked
	int suspended;   //  0 => suspended   |   1 => not suspended
	pcb *next;		//Next PCB in linked list
	pcb *prev;		//Previous PCB in linked list
	//STACK ??  
}


struct queue {
	PCB *head;
	PCB *tail;
	int count;

}

PCB* allocate_pcb(){
	sys_alloc_mem(1024);
	
	reutrn *PCB;
}

PCB* setup_pcb(char name[30], int class, inst priority){

	struct PCB *new_pcb = allocate_pcb();

	if(class == 1){

	} else if(class == 0) {
		
	}else{
		//has to be 1 or 0
		return NULL;
	}
}
