

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

int create_pcb(); // may need to add parameters... for now we get the data from user input within the function

int delete_pcb(char name[30]);

int block_pcb(char name[30]);

int unblock_pcb(char name[30]);

int suspend_pcb(char name[30]);

int resume_pcb(char name[30]);

int show_pcb(char name[30]);

int show_blocked();

int show_ready();

int set_pcb_priority(char name[30], int new_priority);

typedef struct queue {
	PCB *head;
	PCB *tail;
	int count;

}queue;

