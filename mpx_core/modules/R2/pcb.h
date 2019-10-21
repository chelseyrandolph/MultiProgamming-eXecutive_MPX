

typedef struct PCB {
	char name[17];
	int process_class;	// 0 => system process  |  1 => user process
	int priority;		// 0 - 9,   9 being the highest priority
	int readystate; 	// -1 => blocked  |  0 => ready   |  1 => running
	int suspended;   	//  0 => suspended   |   1 => not suspended
	struct PCB *next;		//Next PCB in linked list
	struct PCB *prev;		//Previous PCB in linked list
	
	unsigned char* bottom_of_stack;
	unsigned char* top_of_stack;
	struct content* content;
	int stack_size; 
}PCB;

typedef struct context {
	u32int gs;
	u32int fs;
	u32int es;
	u32int ds;
	u32int edi;
	u32int esi;
	u32int ebp;
	u32int esp;
	u32int ebx;
	u32int edx;
	u32int ecx;
	u32int eax;
	u32int eip;
	u32int cs;
	u32int eflags;
}context;


//This is an internal function that sets up the pcb memory and returns the size.
PCB* allocate_pcb();

/*This is an internal function that includes a name of process, process class, and priority. This function is used to allocate the memory to the new pcb. The process name must be at least 8 character. The priority must be a number from 0 to 9. The process class must either be 0 for the user or 1 for the system. */
PCB* setup_pcb(char *name, int pclass, int priority);

/*This function is an internal function that frees the pcb's memory with sys_free_mem().*/
int free_pcb(PCB* pcb);

/*This function is an internal function that finds a specific pcb. It checks all four linked lists (ready, blocked, susoend, suspend_block) for the given pcb name.*/
PCB* find_pcb(char *process_name);

/*This function is an internal function that inserts a pcb into the appropriate queue.*/
int insert_pcb(PCB* pcb);

/*This function is an internal function that removes a pcb from the appropriate queue.*/
int remove_pcb(PCB* pcb);

/*This function calls setup_pcb and insert_pcb to insert the new pcb into the appropriate queue. It also checks the data being sent in from the user to make sure it meets the requirements from the system.*/
int create_pcb(); 

/*This function will take a pcb name, find it, remove it, and then free all associated memory.*/
int delete_pcb(char name[30]);

/*This function will take a pcb name the user has given and set its state to blocked.*/
int block_pcb(char name[30]);

/*This function will take a pcb name the user has given and set its state to unblocked.*/
int unblock_pcb(char name[30]);

/*This function will take a pcb name the user has given and set its state to suspended.*/
int suspend_pcb(char name[30]);

/*This function will take a pcb name the user has given and set its state to not suspended.*/
int resume_pcb(char name[30]);

/*This function will take a pcb name the user has given and displays information about its state.*/
int show_pcb(char name[30]);

/*This function shows all the pcbs in the blocked queue.*/
int show_blocked();

/*This function shows all the pcbs in the ready queue.*/
int show_ready();

/*This function shows all the pcbs in the system.*/
int show_all();

/*This function will take a pcb anme the user has given and set its new priority to the given integer.*/
int set_pcb_priority(char name[30], int new_priority);

typedef struct queue {
	PCB *head;
	PCB *tail;
	int count;

}queue;

