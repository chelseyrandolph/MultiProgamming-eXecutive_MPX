/*
   Memory manager header file. Our memory manager uses static
	allocation first fit style insertion.
*/

//Complete Memory Control Block
typedef struct CMCB {
	int size;
	char *name;
	int type; //1 for alloc, 0 for free
	struct CMCB* next;
	struct CMCB* prev;
	void* startAddr;

}CMCB;

//We dont use this
typedef struct LMCB {
	int size;
	int type; //1 for alloc, 0 for free
}LMCB;

/*
	Our memory queue
*/
typedef struct mem_queue {
	int count;
	struct CMCB* head;
	struct CMCB* tail;
}mem_queue;


/*
   Function: initialize heap makes sure the memory queue has the requested amount of bytes 
	for initialization. It nulls the head and tail of allocated list, puts the top of the head
	to the strt of memory
   Return type: u32int returned the starting address of the memory queue
   Parameter: u32int head_size_param determins how much memory will be allocated
*/
u32int init_heap(u32int heap_size_param);

/*
   Function: findCMCB will search through the allocated memory queue and find the 
	desired CMCB pased on the parameter
   Return type: struct CMCB pointer to the desired CMCB
   Parameter: char name of the CMCB the function needs to look for
*/
CMCB* findCMCB(char name[]);

/*
   Function: alloc_mem takes the number of bytes desired, searches through the 
	freeblock queue for a block large enough to support the desired size. 
	When found, the block will be unlinked from the freeblock queue and if the size requested
	takes up the necessary amount to take the full block, then that block will be fully inserted into
	the allocated memory queue. Otherwise it will cut the block into necessary size and insert the 
	allocated memory block into the allocated_mem queue and the left over free block back into the
	free block queue with the insert function.
   Return type: u32int address returns the address after the block inserted by taking start address and adding size of block
   Parameter: u32int number of bytes requested
*/
u32int *alloc_mem(u32int num_bytes);

/*
   Function: This takes te address desired to free the block, transverses through the allocated block
	queue, find the address, nullifies the block and reinserts it back into the free memory queue.
	It also calls merge_block to make sure it can merge any two free blocks together.
   Return type: int depending on if it was successful 1 = success 0 = fail -1 = error.
   Parameter: address pointer
*/
int free_mem(void *addr);

/*
   Function: merge_free_blocks takes a pointer to a newly freed
	block, then checks to see if there is an alloc block
	address between it and its next or it and its prev, 
	and then merges them if not	
   Return type: no return type
   Parameter: pointer CMCB freeblock desired to be merged
*/
void merge_free_blocks(CMCB *freeblock);

/*
   Function: checks is allocated memory queue head is null.
   Return type: int 1 is succesful 0 is failure
   Parameter: none
*/
int isEmpty();

/*
   Function: void function that prints free memory queue
   Return type: none, but prints out list in a pretty fashion
   Parameter: none. user command
*/
void show_free_mem();

/*
   Function: void function that prints allocated memory queue
   Return type: none, but prints out list in a pretty fashion
   Parameter: none. user command
*/
void show_alloc_mem();

/*
   Function: void function that unlinks the memory control block depending on where it is in the list
   Return type: none
   Parameter: mcb pointer desired to unlink
*/
void unlink(CMCB* mcb);

/*
   Function: void function that inserts the memory depending on its type into the associated queue
   Return type: none
   Parameter: mcb pointer desired to insert
*/
void insert_mem(CMCB* mcb);


void function();
