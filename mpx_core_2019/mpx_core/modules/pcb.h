

typedef struct PCB {
	char name[30];
	int process_class;	// 0 => system process  |  1 => user process
	int priority;		// 0 - 9,   9 being the highest priority
	int readystate; 	// -1 => blocked  |  0 => ready   |  1 => running
	int suspended;   	//  0 => suspended   |   1 => not suspended
	struct PCB *next;		//Next PCB in linked list
	struct PCB *prev;		//Previous PCB in linked list
	
	unsigned char stack[1024];
	unsigned char *top_of_stack;
	int stack_size; 
}PCB;

PCB* allocate_pcb();

PCB* setup_pcb(char *name, int pclass, int priority);

int free_pcb(PCB* pcb);

PCB* find_pcb(char *process_name);

int insert_pcb(PCB* pcb);

int remove_pcb(PCB* pcb);

typedef struct queue {
	PCB *head;
	PCB *tail;
	int count;

}queue;

