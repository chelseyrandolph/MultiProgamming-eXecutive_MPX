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
#include "../../kernel/mem/heap.c"
#include "../R1/time.h"
#include "../../lib/colortext.h"

CMCB *top_of_heap;
LMCB *bottom_of_heap;

mem_queue allocated_block_list = {.head = NULL, .tail = NULL, .count = 0};
mem_queue free_block_list = {.head = NULL, .tail = NULL, .count = 0};

int heap_size = 1000;	// make this 50,000 at some point
u32int start_of_mem;

int init_heap(u32int heap_size_param){
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

void alloc_mem(int num_bytes){
	int total_bytes = num_bytes + sizeof(CMCB) + sizeof(LMCB);
	CMCB *temp = free_block_list.head;
	
	while(temp != NULL){
		if(temp->size >= total_bytes && temp->type == 0){	//if the block is big enough AND FREE
			// its big enough, allocate mem here
			/*
				HOW DO WE ALLOC?
			*/
		}else if(temp == free_block_list.tail){	// If we get to the tail and its still not big enough
			write_text_bold_yellow("Not enough free memory... \n");
		}else{	// move onto the next CMCB
			temp = temp->next;
		}
	}
}

void free_mem(char addr[]){

	CMCB *temp = allocated_block_list.head;
	while(temp != NULL){
		if(temp->startAddr == addr){
			strcpy(temp->name, NULL);
			temp->type = 0;
		}
		temp = temp -> next;
	}
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




