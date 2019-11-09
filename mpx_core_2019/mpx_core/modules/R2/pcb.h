
/*
This is a struct for a PCB. 
*/
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


/*
This is a struct for the context. 
*/
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


/*
allocate_pcb is an internal function that sets up the pcb memory and returns the size.
Return Type: PCB
Parameters: None
*/
PCB* allocate_pcb();


/*
setup_pcb is an internal function that includes a name of process, process class, and priority. This function is used to allocate the memory to the new pcb. The process name must be at least 8 character. The priority must be a number from 0 to 9. The process class must either be 0 for the user or 1 for the system. 
Return Type: PCB
Parameters: char*: name of PCB
		    int: class type of PCB
		    int: priority of PCB
*/
PCB* setup_pcb(char *name, int pclass, int priority);


/*
free_pcb is an internal function that frees the pcb's memory with sys_free_mem().
Return Type: int
Parameters: PCB: the PCB you're trying to free
*/
int free_pcb(PCB* pcb);

/*
find_pcb is an internal function that finds a specific pcb. It checks all four linked lists (ready, blocked, susoend, suspend_block) for the given pcb name.
Return Type: PCB
Parameters: char*: the name of the PCB you're trying to find
*/
PCB* find_pcb(char *process_name);


/*
insert_pcb is an internal function that inserts a pcb into the appropriate queue.
Return Type: int
Parameters: PCB: the PCB you're trying to insert
*/
int insert_pcb(PCB* pcb);


/*
remove_pcb is an internal function that removes a pcb from the appropriate queue.
Return Type: int
Parameters: PCB: the PCB you're trying to remove
*/
int remove_pcb(PCB* pcb);


/*
create_pcb calls setup_pcb and insert_pcb to insert the new pcb into the appropriate queue. It also checks the data being sent in from the user to make sure it meets the requirements from the system.
Return Type: int
Parameters: none
*/
int create_pcb(); 


/*
delete_pcb will take a pcb name, find it, remove it, and then free all associated memory.
Return Type: int
Parameters: char[30]: the name of the PCB you're trying to delete
*/
int delete_pcb(char name[30]);

/*
block_pcb will take a pcb name the user has given and set its state to blocked.
Return Type: int
Parameters: char[30]: the name of the PCB you're trying to block
*/
int block_pcb(char name[30]);


/*
unblock_pcb will take a pcb name the user has given and set its state to unblocked.
Return Type: int
Parameters: char[30]: the name of the PCB you're trying to unblock
*/
int unblock_pcb(char name[30]);


/*suspend_pcb will take a pcb name the user has given and set its state to suspended.
Return Type: int
Parameters: char[30]: the name of the PCB you're trying to suspend
*/
int suspend_pcb(char name[30]);


/*
resume_pcb will take a pcb name the user has given and set its state to not suspended.
Return Type: int
Parameters: char[30]: the name of the PCB you're trying to resume
*/
int resume_pcb(char name[30]);


/*
show_pcb will take a pcb name the user has given and displays information about its state.
Return Type: int
Parameters: char[30]: the name of the PCB you're trying to display to the user
*/
int show_pcb(char name[30]);


/*
show_blocked shows all the pcbs in the blocked queue.
Return Type: int
Parameters: none
*/
int show_blocked();


/*show_ready shows all the pcbs in the ready queue.
Return Type: int
Parameters: none
*/
int show_ready();


/*show_all shows all the pcbs in the system.
Return Type: int
Parameters: none
*/
int show_all();


/*set_pcb_priority will take a pcb anme the user has given and set its new priority to the given integer.
Return Type: int
Parameters: char[30]: the name of the PCB
            int: the new priority for the PCB
*/
int set_pcb_priority(char name[30], int new_priority);


/*
This is a struct for a queue. 
*/
typedef struct queue {
	PCB *head;
	PCB *tail;
	int count;

}queue;

