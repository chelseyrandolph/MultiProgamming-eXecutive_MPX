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

CMCB *top_of_heap;
LMCB *bottom_of_heap;

mem_queue allocated_block_list = {.head = NULL, .tail = NULL, .count = 0};
mem_queue free_block_list = {.head = NULL, .tail = NULL, .count = 0};

int heap_size = 50000;	// make this 50,000 at some point
u32int start_of_mem;

// Each word is 4 bytes, you wouldn't want to break a block on a byte
int MINIMMUM_FREE_BLOCK_SIZE = 4;

int init_heap(u32int heap_size_param){

	//Rounds up to the next full word which is equal to 4 bytes
	int roundUp = heap_size_param % 4;
	if(roundUp > 0){
		heap_size_param = heap_size_param + (4 - roundUp);
	}
	u32int start_of_mem = kmalloc(heap_size_param + sizeof(CMCB) + sizeof(LMCB));
	top_of_heap = (CMCB*)start_of_mem;
	bottom_of_heap = (LMCB*)(start_of_mem + sizeof(CMCB) + heap_size_param);
	strcpy(top_of_heap->startAddr, itoa(start_of_mem)); // + sizeof??
	top_of_heap->type = 0;
	bottom_of_heap->type = 0;
	top_of_heap->size = heap_size_param;	//Doesnt include size of overhead
	bottom_of_heap->size = heap_size_param;
	top_of_heap->next = NULL;
	top_of_heap->prev = NULL;
	free_block_list.head = top_of_heap;
	free_block_list.tail = top_of_heap;
	if(start_of_mem == NULL){
		write_text_red("init_heap failure... \n");
		return 0;
	}
	return heap_size_param;
}

u32int alloc_mem(u32int num_bytes){

// ------------------------ Added This ------------------------

	CMCB *temp = free_block_list.head;

	while(temp->size < (int) num_bytes){
		//No block can be found to allocate
		if(temp->next == NULL){
			write_text_red("No block available for allocation.\n");
		}
		temp = temp->next;
	}
	//unlink the free block
	unlink(temp);
	
	if((int) (temp->size - num_bytes - sizeof(CMCB) - sizeof(LMCB)) < MINIMMUM_FREE_BLOCK_SIZE){
		num_bytes = temp->size;
		temp->type = 1;
		
		//Change the LMCB to allocated
		LMCB* LMCBEnd = (LMCB*)(((int) temp->startAddr) + num_bytes);
		LMCBEnd->type = 1;

		bottom_of_heap->size = bottom_of_heap->size - num_bytes - sizeof(CMCB) - sizeof(LMCB);
		

		strcpy(temp->startAddr,itoa(sizeof(temp) + sizeof(CMCB)));
		
		insert(temp);
	}else{
		temp->type = 1;
		//New LMCB for the allocated block
		LMCB* LMCBEnd = (LMCB*) ((int)(temp->startAddr) + num_bytes);
		LMCBEnd->type = 1;
		LMCBEnd->size = num_bytes;

		//Create a new free block
		CMCB* freeBlock = (CMCB*)((int)(LMCBEnd) + sizeof(LMCB));
		freeBlock->type = 0;
		freeBlock->size = temp->size - num_bytes - sizeof(CMCB) - sizeof(LMCB);
		

		strcpy(freeBlock->startAddr, itoa(sizeof(freeBlock) + sizeof(CMCB)));

		//Update the end of the heap
		bottom_of_heap->size = bottom_of_heap->size - num_bytes;
		
		temp->size = num_bytes;
		
		insert(temp);
		insert(freeBlock);
	}
	
	return (u32int) temp->startAddr;
	
/*		
while(temp != NULL){
		if(temp->size >= total_bytes && temp->type == 0){	//if the block is big enough AND FREE
			// its big enough, allocate mem here
			
				//HOW DO WE ALLOC?
			
		}else if(temp == free_block_list.tail){	// If we get to the tail and its still not big enough
			write_text_bold_yellow("Not enough free memory... \n");
		}else{	// move onto the next CMCB
			temp = temp->next;
		}
	}*/
}

int free_mem(void *addr){
	
	CMCB *temp = allocated_block_list.head;
	while(temp != NULL){
		if(temp->startAddr == addr){
			strcpy(temp->name, NULL);
			temp->type = 0;
		}
		temp = temp -> next;
	}
// ------------------------ Added This ------------------------
	if(temp == NULL){
		write_text_bold_red("Allocated block not found.\n");
	}
	
	unlink(temp);
	
	LMCB* end = (LMCB*)(((int)temp->startAddr) + temp->size);
	
	//Check for adjacement blocks
	CMCB* block_after = (CMCB*) ((int) end + sizeof(LMCB));
	LMCB* end_after = (LMCB*) (((int) block_after->startAddr) + block_after->size);
	CMCB* temp_after = free_block_list.head;

	while(temp_after != NULL && temp_after != block_after){
		temp_after = temp_after->next;
	}

	LMCB* end_before = (LMCB*) ((int) temp - sizeof(LMCB));
	CMCB* block_before = (CMCB*) ((int) end_before - end_before->size - sizeof(CMCB));
	CMCB* temp_before = free_block_list.head;

	while(temp_before != NULL && temp_before != block_before){
		temp_before = temp_before->next;
	}

	//No adjacement blocks
	if(temp_after == NULL && temp_before == NULL){
		temp->type = 0;
		end->type = 0;
		insert(temp);
		return 0;
	}
	//There is an adjacement block before the free block
	else if(temp_after != NULL && temp_before == NULL){
		unlink(block_before);
		block_before->type = 0;
		block_before->size = block_before->size + sizeof(CMCB) + sizeof(LMCB) + temp->size;
		end->type = 0;
		insert(block_before);
		return 0;
	}
	//There is an adjacement block after the free block
	else if(temp_after != NULL && temp_before == NULL){
		temp->type = 0;
		unlink(block_after);
		temp->size = temp->size + sizeof(CMCB) + sizeof(LMCB) + block_after->size;
		end_after->type = 0;
		insert(temp);
		return 0;
	}	
	//There is an adjacement block before and after the free block
	else{
		unlink(block_before);
		unlink(block_after);
		block_before->type = 0;
		block_before->size = block_before->size + sizeof(CMCB) + sizeof(LMCB) + sizeof(CMCB) + sizeof(LMCB) + block_after->size + temp->size;
		end_after->type = 0;
		end_after->size = block_before->size;
		insert(block_before);
		return 0;
	}

	return 0;
}

int isEmpty(){
	if(allocated_block_list.count == 0){
		write_text("true\n");
		return 1;
	}
	write_text("false\n");	
	return 0;
}

void show_free_mem(){
	write_text_bold_cyan("FREE MEMORY\n");
	CMCB *temp = free_block_list.head;
	while(temp != NULL){
		write_text_bold_blue(itoa(temp->size));
		write_text("	");
		write_text_blue(temp->startAddr);
		write_text("	");
		if(temp->type == 0){
			write_text_blue("FREE");
		}else{
			write_text_blue("ALLOCATED");
		}
		write_text("\n");
		temp = temp->next;
	}
}

void show_alloc_mem(){
	write_text_bold_cyan("ALLOCATED MEMORY\n");
	CMCB *temp = allocated_block_list.head;
	while(temp != NULL){
		write_text_bold_blue(temp->name);
		write_text("	");
		write_text_blue(itoa(temp->size));
		write_text("	");
		write_text_blue(temp->startAddr);
		write_text("	");
		if(temp->type == 0){
			write_text_blue("FREE");
		}else{
			write_text_blue("ALLOCATED");
		}
		write_text("\n");
		temp = temp->next;
	}
}

void unlink(CMCB* mcb){
	//Unlinking the head of free 
	if(mcb == free_block_list.head){
		free_block_list.head = mcb->next;
		mcb->next = NULL;
		mcb->prev = NULL;
		return;
	}
	//Unlinking the head of allocated 
	else if(mcb == allocated_block_list.head){
		allocated_block_list.head = mcb->next;
		mcb->next = NULL;
		mcb->prev = NULL;
		return;
	}
	// mcb is the last item in the list
	if(mcb->next == NULL){
		mcb->prev->next = NULL;
		mcb->prev = NULL;
		return;
	}
	// mcb is somewhere else
	mcb->next->prev = mcb->prev;
	mcb->prev->next = mcb->next;
	mcb->next = NULL;
	mcb->prev = NULL;
	return;

}

void insert(CMCB* mcb){
	CMCB* list = NULL;
	//Determine which list we need to insert into.
	if(mcb->type == 0){
		list = free_block_list.head;
	}else if(mcb->type == 1){
		list = allocated_block_list.head;
	}

	//If the list is empty, set it to the head
	if(list == NULL){
		if(mcb->type == 0){
			free_block_list.head = mcb;
		}else{
			allocated_block_list.head = mcb;
		}
		return;
	}

	CMCB* temp = list;
	
	//Inserting a new head into the list
	if((int) mcb < (int) temp){
		mcb->next = temp;
		mcb->next->prev = mcb;
		if(mcb->type == 0){
			free_block_list.head = mcb;
		}else if(mcb->type == 1){
			allocated_block_list.head = mcb;
		}
		return;
	}
	
	while((int) mcb > (int) temp && temp->next != NULL){
		temp = temp->next;
	}

	//Inserting a new tail
	if(temp->next == NULL && (int) mcb > (int) temp){
		temp->next = mcb;
		mcb->prev = temp;
		return;
	}

	mcb->next = temp;
	mcb->prev = temp->prev;
	temp->prev->next = mcb;
	temp->prev = mcb;
	return;
}





