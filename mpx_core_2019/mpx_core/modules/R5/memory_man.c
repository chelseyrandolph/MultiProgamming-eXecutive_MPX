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
#include "hex.h"

CMCB *top_of_heap = NULL;
LMCB *bottom_of_heap = NULL;
u32int start_of_mem = NULL;

mem_queue allocated_block_list = {.head = NULL, .tail = NULL, .count = 0};
mem_queue free_block_list = {.head = NULL, .tail = NULL, .count = 0};

// Each word is 4 bytes, you wouldn't want to break a block on a byte
int MINIMUM_FREE_BLOCK_SIZE = 4;

u32int init_heap(u32int heap_size_param){
	write_text_bold_magenta("INSIDE INIT_HEAP\n");

	//Rounds up to the next full word which is equal to 4 bytes
	int roundUp = heap_size_param % 4;
	if(roundUp > 0){
		heap_size_param = heap_size_param + (4 - roundUp);
	}

	//Start of memory is the size of the bytes, CMCB, and LMCB to cover overhead.
	start_of_mem = kmalloc(heap_size_param + sizeof(CMCB));

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
	top_of_heap->size = heap_size_param;
	//CMCB start address is equal to the CMCB's size and the size of the CMCB struct itself.
	top_of_heap->startAddr = (void*) (start_of_mem);
	top_of_heap->next = NULL;
	top_of_heap->prev = NULL;

	//The free block list is equal to the heap size. 
	free_block_list.head = top_of_heap;
	free_block_list.tail = top_of_heap;



	return start_of_mem;
}

CMCB* findCMCB(char name[]){
	CMCB* cmcb = allocated_block_list.head;
	while(cmcb != NULL){
		if(cmcb->name == name){
			return cmcb;
		}else{
			cmcb = cmcb->next;
		}
	}
	write_text_bold_blue("memory_man/findPCB: ");
	write_text_red("MEMORY BLOCK NOT FOUND\n");
	return NULL;
}

u32int *alloc_mem(u32int num_bytes){

	int roundUp = num_bytes % 4;
	if(roundUp > 0){
		num_bytes = num_bytes + (4 - roundUp);
	}

	int blocksize = (int)(num_bytes) + sizeof(CMCB); //blocksize accounts for overhead (readability)
	CMCB *temp = free_block_list.head;
	int free_bytes = temp->size;

	while(temp->size < (int) num_bytes){
		if(temp->next == NULL){
			write_text_red("No Block available for allocation");
			return NULL;
		}
		temp = temp->next;
	}

	unlink(temp);
	
	if((temp->size - blocksize) < MINIMUM_FREE_BLOCK_SIZE){
		num_bytes = temp->size;
		temp->type = 1;
		temp->startAddr = (void*)(start_of_mem + sizeof(CMCB));
		bottom_of_heap->size = bottom_of_heap->size - blocksize;
		insert_mem(temp);
	}else{	
		//allocated block 
		temp->type = 1;
		temp->size = num_bytes;
		temp->startAddr = temp->startAddr;
		
		//free block
		CMCB *freeblock = (CMCB*)(temp->startAddr + blocksize);
		freeblock->type = 0;
		freeblock->size = free_bytes - blocksize;
		freeblock->startAddr = temp->startAddr + blocksize;

		insert_mem(temp);
		insert_mem(freeblock);
	}
	PCB* pcb = (PCB*)(temp->startAddr + sizeof(CMCB));
	temp->name = pcb->name;
	return temp->startAddr + sizeof(CMCB);
}

int free_mem(void *addr){

	//Set temp to the allocate list
	CMCB *temp = allocated_block_list.head;
	//Traverse through the list until you find the correct block to free.
	while(temp != NULL){
		if(temp->startAddr == addr){
			unlink(temp);
			strcpy(temp->name, NULL);
			temp->type = 0;
			insert_mem(temp);
			show_free_mem();
			merge_free_blocks(temp);
			return 0;
		}
		temp = temp -> next;
	}

	//If temp is null, alert user and return
	if(temp == NULL){
		write_text_red("ERROR: Block not found.\n");
		return -1;
	}
	
	return 0;
}

/////////////////////////////////////////////////////////
// merge_free_blocks takes a pointer to a newly freed  //
// block, then checks to see if there is an alloc block//
// address between it and its next or it and its prev, //
// and then merges them if not						   //
/////////////////////////////////////////////////////////

void merge_free_blocks(CMCB *freeblock){
	write_text_magenta("INSIDE MERGE\n");
	//CMCB* newFreeBlock = free_block_list.head;
	int newSize; 
	void* newStartAddr;
	int merge = 1;

	if(freeblock->next != NULL){
		CMCB* temp = allocated_block_list.head;
		while(temp != NULL){
			//If a alloc block is found between the two free blocks then do not merge them
			if((int)(temp->startAddr) > (int)(freeblock->startAddr)
			 && (int)(temp->startAddr) < (int)(freeblock->next->startAddr)){
				write_text_magenta("NOT MERGING: 1\n");
				merge = 0;	// Freeblock      Allocated    Free->next
				break;
			}else{
				temp = temp->next;
			}
		}
	}


	if(merge == 1 && freeblock->next != NULL){	// no alloc block was found between the two free blocks so merge!	
		write_text_magenta("MERGING: 1\n");
		newSize = freeblock->size + freeblock->next->size + sizeof(CMCB);
		newStartAddr = freeblock->startAddr;
		unlink(freeblock->next);
		unlink(freeblock);

		freeblock->size = newSize;
		freeblock->startAddr = newStartAddr;
		freeblock->type = 0;

		insert_mem(freeblock);
		write_text_magenta("RETURN 1\n");
		return;
	}


	merge = 1;
	if(freeblock->prev != NULL){
		CMCB* temp = allocated_block_list.head;
		while(temp != NULL){
			//If a alloc block is found between the two free blocks then do not merge them
			if((int)(temp->startAddr) > (int)(freeblock->prev->startAddr)
			 && (int)(temp->startAddr) < (int)(freeblock->startAddr)){
				write_text_magenta("NOT MERGING: 2\n");
				merge = 0; // Free->prev     Freeblock     Allocated
			}else{
				temp = temp->next;
			}
		}
	}
	if(merge == 1 && freeblock->prev != NULL){ // no alloc block was found between the two free blocks so merge!
		write_text_magenta("MERGING: 2\n");
		newSize = freeblock->prev->size + freeblock->size + sizeof(CMCB);
		newStartAddr = freeblock->prev->startAddr;
		unlink(freeblock->prev);
		unlink(freeblock);

		freeblock->size = newSize;
		freeblock->startAddr = newStartAddr;
		freeblock->type = 0;

		insert_mem(freeblock);
		write_text_magenta("RETRUN 2\n");
		return;
	}

}

int isEmpty(){
	if(allocated_block_list.head == NULL){
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
	CMCB *temp = free_block_list.tail;
	while(temp != NULL){
		write_text("| ");
		char sizestr[11];
		strcpy(sizestr, itoa(temp->size));
		int i;
		for(i = strlen(itoa(temp->size)); i < 10; i++){	
			sizestr[i] = ' ';
		}
		sizestr[10] = '\0';
		write_text_blue(itoa(temp->size));
		//write_text_blue(sizestr);
		write_text(" | ");

		printDecToHex((int)temp->startAddr);
		write_text("   | ");			//TODO this is lazy, fix it
		if(temp->type == 0){
			write_text_blue("  FREE   ");
		}else{
			write_text("\n");
			write_text_red("Error... Allocated Block in the Free List.\n");
			return;
		}
		write_text(" |\n");
		temp = temp->prev;
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
	CMCB *temp = allocated_block_list.tail;
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

		printDecToHex((int)temp->startAddr);
		write_text("   | ");			//TODO this is lazy, fix it
		
		write_text_blue("ALLOCATED");
		
		write_text(" |\n");
		temp = temp->prev;
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
		write_text_bold_yellow("Unlinked head of free .... LEAVING UNLINK 1\n");
		return;
	}
	//Unlinking the head of allocated 
	else if(mcb == allocated_block_list.head){
		allocated_block_list.head = mcb->next;
		mcb->next = NULL;
		mcb->prev = NULL;
		write_text_bold_yellow("Unlinked head of allocated .... LEAVING UNLINK 2\n");
		return;
	}
	// mcb is the last item in the list
	if(mcb->next == NULL){
		mcb->prev->next = NULL;
		mcb->prev = NULL;
		write_text_bold_yellow("Unlink last item in list .... LEAVING UNLINK 3\n");
		return;
	}
	// mcb is somewhere else
	mcb->next->prev = mcb->prev;
	mcb->prev->next = mcb->next;
	mcb->next = NULL;
	mcb->prev = NULL;
	write_text_bold_yellow("Unlinked within list .... LEAVING UNLINK 4\n");
	return;

}

void insert_mem(CMCB* mcb){
	write_text_bold_cyan("INSIDE INSERT\n");
	CMCB* temp = NULL;
	
	if(mcb->type == 0){					//If free
		write_text_cyan("INSIDE INSERT: Type is free\n");
		temp = free_block_list.head;
		if(temp == NULL){			//if list is empty, set head and tail
			write_text_cyan("INSIDE INSERT: Free list empty\n");
			free_block_list.head = mcb;
			free_block_list.tail = mcb;
			free_block_list.count = 1;
			write_text_bold_cyan("LEAVING INSERT: free list empty \n");
		}
	
		while(temp!=NULL){
			if(mcb->startAddr > temp->startAddr){
			write_text_cyan("INSIDE INSERT: Insert at head\n");	
				if(temp->prev == NULL){			//if head
					mcb->next = temp;			//insert at head
					temp->prev = mcb;
					free_block_list.head = mcb;
				}else if(temp->next == NULL){	//if tail
					write_text_cyan("INSIDE INSERT: Insert at tail\n");	
					mcb->prev = temp;			//insert at tail
					temp->next = mcb;
					free_block_list.tail = mcb;
					break;
				}else{						//if neither head nor tail
					write_text("INSIDE INSERT: Insert in between \n");
					mcb->next = temp;			//insert in between
					temp->prev->next = mcb;
					mcb->prev = temp->prev;
					temp->prev = mcb;
					
				}
				free_block_list.count++;		//increment count
				write_text_bold_cyan("LEAVING INSERT: free list \n");
				return;
			}else{
				write_text_cyan("INSIDE INSERT: Insert at head 2\n");	
				mcb->next = temp;			//insert at head
				temp->prev = mcb;
				free_block_list.head = mcb;
			}
			temp = temp->next;
		}
		
	}else if(mcb->type == 1){			//if allocated
			write_text_cyan("INSIDE INSERT: Type is allocated\n");
		temp = allocated_block_list.head;
		if(temp == NULL){			//if list is empty, set head and tail
		write_text_cyan("INSIDE INSERT: Allocated list is empty\n");
			allocated_block_list.head = mcb;
			allocated_block_list.tail = mcb;
			allocated_block_list.count = 1;
			write_text_bold_cyan("LEAVING INSERT: into empty list \n");
		}
		while(temp!=NULL){
			if(mcb->startAddr > temp->startAddr){
				if(temp->prev == NULL){			//if head
					write_text_cyan("INSIDE INSERT: Insert at head\n");
					mcb -> next = temp;			//insert at head
					temp -> prev = mcb;
					allocated_block_list.head = mcb;
				}else if(temp->next == NULL){	//if tail
					write_text_cyan("INSIDE INSERT: Insert at tail\n");
					mcb->prev = temp;			//insert at tail
					temp->next = mcb;
					allocated_block_list.tail = mcb;
				}else{							//if neither head nor tail
					write_text_cyan("INSIDE INSERT: Insert in between\n");
					mcb->next = temp;			//insert in between
					mcb->prev = temp->prev;
					temp->prev = mcb;
					temp->prev->next = mcb;
				}
				allocated_block_list.count++;	//increment count
				write_text_bold_cyan("LEAVING INSERT: Allocated\n");
				return;
			}else{
				temp = temp->next;
			}
		}

	}
	write_text_bold_cyan("LEAVING INSERT: End of function\n");
	return;
}



void function(){
	

	alloc_mem(200);

//	show_alloc_mem();
//	write_text_green("size of pcb: ");
//	write_text_bold_green(itoa(sizeof(PCB)));
//	write_text_bold_green(" bytes\n");
	PCB *pcb = (PCB*)alloc_mem(sizeof(PCB));
	alloc_mem(400);
	PCB *pcb2 = (PCB*)alloc_mem(sizeof(PCB));
	strcpy(pcb2->name , "name5678");
	//PCB *pcb2 = alloc_mem(sizeof(PCB));
	
	strcpy(pcb->name, "name1234");
	//free_mem(pcb2);
	show_free_mem();
	show_alloc_mem();

	free_mem(findCMCB(pcb2->name));

	show_free_mem();
	show_alloc_mem();

	free_mem(findCMCB(pcb->name));

	show_free_mem();
	show_alloc_mem();

}








