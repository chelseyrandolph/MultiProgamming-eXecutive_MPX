#include <string.h>
#include <system.h>
#include <mem/heap.h>
#include <core/io.h>
#include <core/serial.h>
#include <core/tables.h>
#include <core/interrupts.h>
#include "../mpx_supt.h"
#include "../R4/pcb.h"
#include "memory_man.h"
#include "../../include/mem/heap.h"
#include "../R1/time.h"
#include "../../lib/colortext.h"


CMCB *top_of_heap = NULL;
LMCB *bottom_of_heap = NULL;
u32int start_of_mem = NULL;

mem_queue allocated_block_list = {.head = NULL, .tail = NULL, .count = 0};
mem_queue free_block_list = {.head = NULL, .tail = NULL, .count = 0};

// Each word is 4 bytes, you wouldn't want to break a block on a byte
int MINIMMUM_FREE_BLOCK_SIZE = 4;

u32int init_heap(u32int heap_size_param){
	write_text_bold_magenta("INSIDE INIT_HEAP\n");

	//Rounds up to the next full word which is equal to 4 bytes
	int roundUp = heap_size_param % 4;
	if(roundUp > 0){
		heap_size_param = heap_size_param + (4 - roundUp);
	}

	//Start of memory is the size of the bytes, CMCB, and LMCB to cover overhead.
	start_of_mem = kmalloc(heap_size_param + sizeof(CMCB) + sizeof(LMCB));

	//If this is null, then the heap wasn't given the space it needs to set up.
	if(start_of_mem == NULL){
		write_text_red("init_heap failure... \n");
		return -1;
	}

	//Makes sure at the at the start of memory, the allocated block list is null.
	allocated_block_list.head = NULL;
	allocated_block_list.tail = NULL;

	//CMCB is equal to the start of memory, type is free, and the size if the size of the heap.
	top_of_heap = (CMCB*) start_of_mem;
	top_of_heap->type = 0;
	top_of_heap->size = heap_size_param + sizeof(CMCB) + sizeof(LMCB);
	//CMCB start address is equal to the CMCB's size and the size of the CMCB struct itself.
	top_of_heap->startAddr = (void*) (start_of_mem);
	top_of_heap->next = NULL;
	top_of_heap->prev = NULL;

	//The free block list is equal to the heap size. 
	free_block_list.head = top_of_heap;
	free_block_list.tail = top_of_heap;

	//The LMCB is equal to the start of memory, size of CMCB and the heap size, with a type of free.
	bottom_of_heap = (LMCB*)((int)start_of_mem + sizeof(CMCB) + heap_size_param);
	bottom_of_heap->type = 0;

	//The LMCB size of equal to the size of the CMCB at the start of the heap
	bottom_of_heap->size = top_of_heap->size;


	write_text_bold_magenta("LEAVING INIT_HEAP\n");
	write_text("This is the number of bytes initialized in the heap: ");
	write_text_magenta(itoa(top_of_heap->size));
	write_text("\n");
	//Hexadecimal number
	return start_of_mem;
}


u32int alloc_mem(u32int num_bytes){

	write_text_bold_green("INSIDE ALLOC_MEM\n");

	//Rounds up to the next full word which is equal to 4 bytes
	int roundUp = num_bytes % 4;
	if(roundUp > 0){
		num_bytes = num_bytes + (4 - roundUp);
	}

	//Setting the CMCB to the free block list to allocate a new block 
	CMCB *temp = free_block_list.head;
	int free_bytes = temp->size;
	write_text_bold_magenta(itoa(temp->size - sizeof(CMCB) - sizeof(LMCB)));
	write_text_green("ALLOC_MEM 0\n");


	// If the size of the free block is less than the size of the bytes needed to allocate,
	// We iterate through the list until we get to the end. If we reach the end,
	// we will throw and error that there is not enough room to allocate that block and exit.
	while(temp->size < (int) num_bytes){
		write_text_green("ALLOC_MEM 1\n");
		//No block can be found to allocate
		if(temp->next == NULL){
			write_text_red("ERROR: No block available for allocation.\n");
			return -1;
		}
		temp = temp->next;
	}

	write_text_green("ALLOC_MEM 2\n");
	//unlink the block
	unlink(temp);

	write_text_green("ALLOC_MEM 3\n");


	//If the block we are allocating would leave a left over  block not big enough for a word,
	// (a bytes = 4), then we do not split that block.
	if((int)(temp->size - num_bytes - sizeof(CMCB) - sizeof(LMCB)) < MINIMMUM_FREE_BLOCK_SIZE){
		write_text_green("ALLOC_MEM 4\n");
		// Updates num_bytes and change the type to allocated
		num_bytes = temp->size;
		temp->type = 1;
		temp->startAddr = (void*)(start_of_mem + sizeof(CMCB));

		//Change the LMCB to allocated
		LMCB* LMCBEnd = (LMCB*)(temp->startAddr + num_bytes);
		LMCBEnd->type = 1;
		bottom_of_heap->size = bottom_of_heap->size - num_bytes - sizeof(CMCB) - sizeof(LMCB);

		insert(temp);
		write_text_green("ALLOC_MEM 5\n");
	}else{

		//Type is allocated
		temp->type = 1;
		temp->startAddr = (void*)((int)start_of_mem + sizeof(CMCB));


		//New LMCB for the allocated block
		//TODO: FIX THIS
		LMCB* LMCBEnd = (LMCB*) ((int)temp->startAddr + num_bytes);
		write_text_green("ALLOC_MEM 6\n");
		LMCBEnd->type = 1;
		LMCBEnd->size = num_bytes + sizeof(LMCB);
	

		//Create a new free block
		CMCB* freeBlock = (CMCB*)((int)(LMCBEnd) + sizeof(LMCB));
		write_text_green("ALLOC_MEM 7\n");
		freeBlock->type = 0;
		write_text_bold_magenta(itoa(temp->size - sizeof(CMCB) - sizeof(LMCB)));
		freeBlock->size = free_bytes - sizeof(CMCB) - sizeof(LMCB);
		//TODO: FIX THIS ? 
		freeBlock->startAddr = (void*) ((int) freeBlock + sizeof(CMCB));
		freeBlock->next = NULL;
		freeBlock->prev = NULL;
		write_text_green("ALLOC_MEM 8\n");


		//TODO: DO WE NEED THIS ? 
		//Update the end of the heap
		bottom_of_heap->size = bottom_of_heap->size - num_bytes;
		temp->size = num_bytes;
		write_text_green("ALLOC_MEM 9\n");
		//Insert the blocks into the correct lists (temp = allocated, free = free)
		insert(temp);
		insert(freeBlock);
	}

	write_text_bold_green("LEAVING ALLOC_MEM\n");
	return (u32int) temp->startAddr;
	
}

int free_mem(void *addr){
	write_text_bold_blue("INSIDE FREE_MEM\n");

	//Set temp to the allocate list
	CMCB *temp = allocated_block_list.head;
	//Traverse through the list until you find the correct block to free.
	while(temp != NULL){
		if(temp->startAddr == addr){
			write_text_blue("FREE_MEM 0\n");
			strcpy(temp->name, NULL);
			temp->type = 0;
		}
		temp = temp -> next;
	}
// ------------------------ Added This ------------------------
	//If temp is null, throw an error 
	if(temp == NULL){
		write_text_bold_red("ERROR: Allocated block not found.\n");
		return -1;
	}
	write_text_blue("FREE_MEM 1\n");
	unlink(temp);
	
	
	LMCB* end = (LMCB*)(((int)temp->startAddr) + temp->size);
	write_text_blue("FREE_MEM 2\n");
	//Check for adjacement blocks
	//Checking the block after
	CMCB* block_after = (CMCB*) ((int) end + sizeof(LMCB));
	LMCB* end_after = (LMCB*) (((int) block_after->startAddr) + block_after->size);
	CMCB* temp_after = free_block_list.head;

	write_text_blue("FREE_MEM 3\n");
	while(temp_after != NULL && temp_after != block_after){
		temp_after = temp_after->next;
	}

	write_text_blue("FREE_MEM 4\n");
	//Checking the block before
	LMCB* end_before = (LMCB*) ((int) temp - sizeof(LMCB));
	CMCB* block_before = (CMCB*) ((int) end_before - end_before->size - sizeof(CMCB));
	CMCB* temp_before = free_block_list.head;


	write_text_blue("FREE_MEM 5\n");
	//Traverse the lists
	while(temp_before != NULL && temp_before != block_before){
		temp_before = temp_before->next;
	}

	write_text_blue("FREE_MEM 6\n");
	//No adjacement blocks
	// We just insert it as a new free block
	if(temp_after == NULL && temp_before == NULL){
		write_text_blue("FREE_MEM 7\n");
		temp->type = 0;
		end->type = 0;
		insert(temp);
		write_text_bold_blue("LEAVING FREE_MEM 1\n");
		return 0;
	}

	//There is an adjacement block before the free block
	//We put these two blocks together into one free block.
	else if(temp_after != NULL && temp_before == NULL){
		write_text_blue("FREE_MEM 8\n");
		unlink(block_before);
		block_before->type = 0;
		block_before->size = block_before->size + sizeof(CMCB) + sizeof(LMCB) + temp->size;
		end->type = 0;
		insert(block_before);
		write_text_bold_blue("LEAVING FREE_MEM 2\n");
		return 0;
	}

	//There is an adjacement block after the free block
	//We put these two blocks together into one free block.
	else if(temp_after != NULL && temp_before == NULL){
		write_text_blue("FREE_MEM 9\n");
		temp->type = 0;
		unlink(block_after);
		temp->size = temp->size + sizeof(CMCB) + sizeof(LMCB) + block_after->size;
		end_after->type = 0;
		insert(temp);
		write_text_bold_blue("LEAVING FREE_MEM 3\n");
		return 0;
	}	

	//There is an adjacement block before and after the free block
	//We put these three blocks together into one free block.
	else{
		write_text_blue("FREE_MEM 10\n");
		unlink(block_before);
		unlink(block_after);
		block_before->type = 0;
		block_before->size = block_before->size + sizeof(CMCB) + sizeof(LMCB) + sizeof(CMCB) + sizeof(LMCB) + block_after->size + temp->size;
		end_after->type = 0;
		end_after->size = block_before->size;
		insert(block_before);
		write_text_bold_blue("LEAVING FREE_MEM 4\n");
		return 0;
	}
}

//TODO //TODO //TODO //TODO //TODO //TODO //TODO //TODO

// WE HAVE NEVER USED ALLOCATED BLOCK LIST . COUNT
// SHOULD WE JUST CHECK THE HEAD ???

//TODO //TODO //TODO //TODO //TODO //TODO //TODO //TODO
int isEmpty(){
	if(allocated_block_list.count == 0){
		write_text("true\n");
		return 1;
	}
	write_text("false\n");	
	return 0;
}

// I want to make it auto size the columns based on the size of the information. 
// For not I have hard coded it. 
void show_free_mem(){
	write_text("\n\n");
	write_text_bold_cyan("FREE MEMORY\n");
	write_text_bold_blue(" ______________________________________\n");
	write_text_bold_blue("|                                      |\n");
	write_text_bold_blue("|            |    Start    |           |\n");
	write_text_bold_blue("|    Size    |   Address   |    Type   |\n");
	write_text_bold_blue("|______________________________________|\n");
	CMCB *temp = free_block_list.head;
	while(temp != NULL){
		write_text("| ");
		char sizestr[11];
		strcpy(sizestr, itoa(temp->size));
		int i;
		for(i = strlen(itoa(temp->size)); i < 10; i++){	
			sizestr[i] = ' ';
		}
		sizestr[10] = '\0';
		write_text_blue(sizestr);
		write_text(" | ");
		char addrstr[12];
		strcpy(addrstr, itoa((int)temp->startAddr));
		i = 0;
		for(i = strlen(itoa((int)temp->startAddr)); i < 11; i++){	
			addrstr[i] = ' ';
		}
		addrstr[11] = '\0';
		write_text_blue(addrstr);
		write_text(" | ");
		if(temp->type == 0){
			write_text_blue("  FREE   ");
		}else{
			write_text("\n");
			write_text_red("Error... Allocated Block in the Free List.\n");
			return;
		}
		write_text(" |\n");
		temp = temp->next;
	}
		write_text("|______________________________________|\n");
		write_text("\n\n");
}

void show_alloc_mem(){
	write_text("\n\n");
	write_text_bold_cyan("ALLOCATED MEMORY\n");
	write_text_bold_blue(" _________________________________________________________\n");
	write_text_bold_blue("|                                                         |\n");
	write_text_bold_blue("|                  |            |    Start    |           |\n");
	write_text_bold_blue("|       Name       |    Size    |   Address   |    Type   |\n");
	write_text_bold_blue("|_________________________________________________________|\n");
	CMCB *temp = allocated_block_list.head;
	while(temp != NULL){
		write_text("| ");
		char namestr[17];
		strcpy(namestr, temp->name);
		int i;
		for(i = strlen(temp->name); i < 16; i++){	
			namestr[i] = ' ';
		}
		namestr[16] = '\0';
		write_text_blue(namestr);
		write_text(" | ");
		char sizestr[11];
		strcpy(sizestr, itoa(temp->size));
		i = 0;
		for(i = strlen(itoa(temp->size)); i < 10; i++){	
			sizestr[i] = ' ';
		}
		sizestr[10] = '\0';
		write_text_blue(sizestr);
		write_text(" | ");
		char addrstr[12];
		strcpy(addrstr, itoa((int)temp->startAddr));
		i = 0;
		for(i = strlen(itoa((int)temp->startAddr)); i < 11; i++){	
			addrstr[i] = ' ';
		}
		addrstr[11] = '\0';
		write_text_blue(addrstr);
		write_text(" | ");
		if(temp->type == 1){
			write_text_blue("ALLOCATED");
		}else{
			write_text("\n");
			write_text_red("Error... Free Block in the Allocated List.\n");
			return;
		}
		write_text(" |\n");
		temp = temp->next;
	}
		write_text("|_________________________________________________________|\n");
		write_text("\n\n");
}

void unlink(CMCB* mcb){
	write_text_bold_yellow("INSIDE UNLINK\n");
	//Unlinking the head of free 
	if(mcb == free_block_list.head){
		free_block_list.head = mcb->next;
		mcb->next = NULL;
		mcb->prev = NULL;
		write_text_bold_yellow("LEAVING UNLINK 1\n");
		return;
	}
	//Unlinking the head of allocated 
	else if(mcb == allocated_block_list.head){
		allocated_block_list.head = mcb->next;
		mcb->next = NULL;
		mcb->prev = NULL;
		write_text_bold_yellow("LEAVING UNLINK 2\n");
		return;
	}
	// mcb is the last item in the list
	if(mcb->next == NULL){
		mcb->prev->next = NULL;
		mcb->prev = NULL;
		write_text_bold_yellow("LEAVING UNLINK 3\n");
		return;
	}
	// mcb is somewhere else
	mcb->next->prev = mcb->prev;
	mcb->prev->next = mcb->next;
	mcb->next = NULL;
	mcb->prev = NULL;
	write_text_bold_yellow("LEAVING UNLINK 4\n");
	return;

}

void insert(CMCB* mcb){
	write_text_bold_cyan("INSIDE INSERT\n");
	write_text_bold_cyan("INSIDE INSERT 0\n");
	CMCB* list = NULL;
	//Determine which list we need to insert into.
	if(mcb->type == 0){
		write_text_cyan("INSIDE INSERT 1\n");
		list = free_block_list.head;
		free_block_list.count++;
	}else if(mcb->type == 1){
		write_text_cyan("INSIDE INSERT 2\n");
		list = allocated_block_list.head;
		allocated_block_list.count++;
	}

	//If the list is empty, set it to the head
	if(list == NULL){
		if(mcb->type == 0){
			write_text_cyan("INSIDE INSERT 3\n");
			free_block_list.head = mcb;
			free_block_list.count++;
		}else{
			write_text_cyan("INSIDE INSERT 4\n");
			allocated_block_list.head = mcb;
			allocated_block_list.count++;
		}
		write_text_bold_cyan("LEAVING INSERT 0\n");
		return;
	}

	CMCB* temp = list;
	write_text_cyan("INSIDE INSERT 5\n");

	//Inserting a new head into the list
	if(mcb->size < temp->size){
		write_text_cyan("INSIDE INSERT 6\n");
		mcb->next = temp;
		write_text_cyan("INSIDE INSERT 7\n");
		mcb->next->prev = mcb;
		if(mcb->type == 0){
			write_text_cyan("INSIDE INSERT 8\n");
			free_block_list.head = mcb;
			free_block_list.count++;
		}else if(mcb->type == 1){
			write_text_cyan("INSIDE INSERT 9 \n");
			allocated_block_list.head = mcb;
			allocated_block_list.count++;
		}
		write_text_bold_cyan("LEAVING INSERT 1\n");
		return;
	}

		//Traversing through the list if the new mcb is larger than the current mcb
	while(mcb->size > temp->size && temp->next != NULL){
		write_text_cyan("INSIDE INSERT 10\n");
		temp = temp->next;
	}

	//Inserting a new tail
	if(temp->next == NULL && mcb->size > temp->size){
		write_text_cyan("INSIDE INSERT 11 \n");
		temp->next = mcb;
		mcb->prev = temp;
		if(mcb->type == 0){
			write_text_cyan("INSIDE INSERT 8\n");
			free_block_list.head = mcb;
			free_block_list.count++;
		}else if(mcb->type == 1){
			write_text_cyan("INSIDE INSERT 9 \n");
			allocated_block_list.head = mcb;
			allocated_block_list.count++;
		}
		write_text_bold_cyan("LEAVING INSERT 2\n");
		return;
	}

	//Inserting into the list in a place between the head and tail
	write_text_cyan("INSIDE INSERT 12\n");
	if(mcb->type == 0){
		mcb->next = temp;
		mcb->prev = temp->prev;
		temp->prev->next = mcb;
		temp->prev = mcb;
		free_block_list.count++;
	}else if(mcb->type == 1){
		mcb->next = temp;
		mcb->prev = temp->prev;
		temp->prev->next = mcb;
		temp->prev = mcb;
		allocated_block_list.count++;
	}
	write_text_bold_cyan("LEAVING INSERT 4\n");
	return;
}


void function(){
	write_text("allocating 10 bytes....\n");
	alloc_mem(10);
	write_text("allocated 10 bytes.\n");
	show_alloc_mem();
	show_free_mem();
	write_text("allocating 15 bytes....\n");
	alloc_mem(15);
	write_text("allocated 15 bytes.\n");
	show_alloc_mem();
	show_free_mem();
	write_text("allocating 20 bytes....\n");
	alloc_mem(20);
	write_text("allocated 20 bytes.\n");
	show_alloc_mem();
	show_free_mem();
	//write_text("freeing 15 bytes\n");
	//free_mem((void*)52);
	//write_text("freed 15 bytes\n");
	write_text("done.\n");
	
}








