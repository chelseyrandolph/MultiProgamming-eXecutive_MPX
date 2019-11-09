#include <system.h>
#include <core/io.h>
#include <core/serial.h>
#include <string.h>
#include "../mpx_supt.h"
#include "comhand.h"
#include "time.h"
#include "date.h"
#include "comhandsupport.h"
#include "../../lib/colortext.h"
#include "../R4/Dispatcher.h"
#include "../R4/Alarm.h"
#include "../R5/memory_man.h"




void version(){			//Prints the current version
	static char version[20] = "MPX-OS Version 2.70\n";
	write_text_bold_green(version);
}

void displayAllCommands(){
	static int msg_size = 100;
	static char message[100] = "\033[1;34mAvaiable Commands are\033[0m \n";
	sys_req(WRITE, DEFAULT_DEVICE, message, &msg_size);
	int k;
	int comLength = sizeof(helpcommands)/sizeof(helpcommands[0]);
	int one = 1;
	for(k=0; k<comLength; k++){
		int tempSize = 50;
		
		sys_req(WRITE,DEFAULT_DEVICE,helpcommands[k],&tempSize);
		sys_req(WRITE,DEFAULT_DEVICE,"\n",&one);
	}
	sys_req(WRITE,DEFAULT_DEVICE,"\n",&one);
}

void clear(){
	static int size = 10;
	sys_req(WRITE, DEFAULT_DEVICE, "\033[2J", &size);
	sys_req(WRITE, DEFAULT_DEVICE, "\033[1;1H", &size);
}

int inputHelp(char helpBuffer[]){
	int tempSize = 45;
	int i = -1;
	int k;
	int comLength = sizeof(commands)/sizeof(commands[0]);
	for(k=0; k<comLength; k++){
		if(strcmp(helpBuffer, commands[k])==0){
			i = k+1;
		}
	}
	
		switch(i){
			case 1: sys_req(WRITE, DEFAULT_DEVICE, helpcommands[i-1], &tempSize);
					write_text("\nVersion displays the version of MPX-OS currently running.\n\n");
					break;

			case 2: sys_req(WRITE, DEFAULT_DEVICE, helpcommands[i-1], &tempSize);  
					write_text("\nHelp displays necessary information on the function. \n\n");					
					break;

			case 3: sys_req(WRITE, DEFAULT_DEVICE, helpcommands[i-1], &tempSize);  
					write_text("\nShutdown will power off MPX-OS, you will first be asked for confirmation before powering off by selecting (1) to confirm, or (2) to cancel.\n\n");					
					break;

			case 4:	sys_req(WRITE, DEFAULT_DEVICE, helpcommands[i-1], &tempSize); 
					write_text("\nDisplays the current time of day.\n\n");
					break;

			case 5: sys_req(WRITE, DEFAULT_DEVICE, helpcommands[i-1], &tempSize);
					write_text("\nDisplays the current date.\n\n");  
					break;

			case 6: sys_req(WRITE, DEFAULT_DEVICE, helpcommands[i-1], &tempSize); 
					write_text("\nPrompts the user for an hour, minute, and second to change the MPX clock to.\n\n"); 
					break;

			case 7: sys_req(WRITE, DEFAULT_DEVICE, helpcommands[i-1], &tempSize);
					write_text("\nPrompts the user for a month, day, and year to change the MPX date to.\n\n");
					break;

			case 8: sys_req(WRITE, DEFAULT_DEVICE, helpcommands[i-1], &tempSize);  
					write_text("\nCreates a PCB.\n\n"); 
					break;
			
			case 9: sys_req(WRITE, DEFAULT_DEVICE, helpcommands[i-1], &tempSize);
					write_text("\nShows all PCBs Created.\n\n");   
					break;

			case 10: sys_req(WRITE, DEFAULT_DEVICE, helpcommands[i-1], &tempSize); 
					 write_text("\nShows all the blocked PCBs.\n\n"); 
					 break;

			case 11: sys_req(WRITE, DEFAULT_DEVICE, helpcommands[i-1], &tempSize); 
					 write_text("\nShows PCBs that are ready for execution.\n\n"); 
					 break;

			case 12: sys_req(WRITE, DEFAULT_DEVICE, helpcommands[i-1], &tempSize); 
					 write_text("\nShows a single, user chosen PCB.\n\n"); 
					 break;
	
			case 13: sys_req(WRITE, DEFAULT_DEVICE, helpcommands[i-1], &tempSize); 
					 write_text("\nDeletes a PCB from the appropriate queue and then free all associated memory.\n\n");
					 break;
			
			case 14: sys_req(WRITE, DEFAULT_DEVICE, helpcommands[i-1], &tempSize); 
					 write_text("\nPlaces a PCB in the suspended state.\n\n"); 
					 break;

			case 15: sys_req(WRITE, DEFAULT_DEVICE, helpcommands[i-1], &tempSize);
					 write_text("\nPlaces a PCB in the not suspended state.\n\n"); 
					 break;
			/*
			case 16: sys_req(WRITE, DEFAULT_DEVICE, helpcommands[i-1], &tempSize); 
					 write_text("\nSets a PCB's state to blocked.\n\n"); 
					 break;

			case 17: sys_req(WRITE, DEFAULT_DEVICE, helpcommands[i-1], &tempSize); 
					 write_text("\nSets a PCB's state to unblocked.\n\n"); 
					 break;
			*/

			case 16: sys_req(WRITE, DEFAULT_DEVICE, helpcommands[i-1], &tempSize); 
					 write_text("\nSets a PCB's priority.\n\n"); 
					 break;
/*
			case 17: sys_req(WRITE, DEFAULT_DEVICE, helpcommands[i-1], &tempSize);
					 write_text("\nCauses comhand to yield to other processes.\n\n");
					 break;
*/
			case 17: sys_req(WRITE, DEFAULT_DEVICE, helpcommands[i-1], &tempSize); 
					 write_text("\nLoads test processes.\n\n"); 
					 break;

			case 18: sys_req(WRITE, DEFAULT_DEVICE, helpcommands[i-1], &tempSize); 
					 write_text("\nThis clears the terminal.\n\n"); 
					 break;

			case 19: sys_req(WRITE, DEFAULT_DEVICE, helpcommands[i-1], &tempSize); 
					 write_text("\nThis creates an alarm.\n\n");
					 break;

			case 20: sys_req(WRITE, DEFAULT_DEVICE, helpcommands[i-1], &tempSize); 
					 write_text("\nThis creates the infinite process.\n\n");
					 break;

			case 21: sys_req(WRITE, DEFAULT_DEVICE, helpcommands[i-1], &tempSize); 
					 write_text("\nThis shows the free memory blocks.\n\n");
					 break;

			case 22: sys_req(WRITE, DEFAULT_DEVICE, helpcommands[i-1], &tempSize); 
					 write_text("\nThis shows the allocated memory blocks.\n\n");
					 break;

			default: displayAllCommands();
		}
		return 0;

}





int shutDown(){
	static int size = 100;
	static char prompt[100] = "Are you sure? (\033[0;32mY\033[0m/\033[0;31mN\033[0m)\n";
	int ansInt = 2;
	static char ans[2];
	memset(ans, '\0', 2);
	sys_req(WRITE, DEFAULT_DEVICE, prompt, &size);
	sys_req(READ, DEFAULT_DEVICE, ans, &ansInt);
	
	if(strcmp(ans, "Y")==0){
		PCB *infinite = find_pcb("infinite_process");
		if(infinite != NULL && infinite->suspended == 0){
			write_text_bold_yellow("Cannot shutdown while infinite process is not suspended\n");
			return 0;
		}	
		write_text_red("MPX is shutting down \n");
		return 1;
	}else if(strcmp(ans, "N")==0){
		write_text_bold_yellow("Shutdown canceled\n");
		return 0;
	}else{
		write_text_bold_red("Not valid command, aborting...\n");
		return 0;
	}
}



int comhand(){
	int quit = 0;
	while(!quit){
		char *cmdBuffer = sys_alloc_mem(100);
		int bufferSize;
		
	
//		Get a command
		memset(cmdBuffer, '\0', 100);
		bufferSize = 99;
		sys_req(READ,DEFAULT_DEVICE,cmdBuffer,&bufferSize);
		
//		Tokenize buffer. 
//		This set of instructions will break the buffer on white space and put it inside a *array
//		0 index is always the command wihle everything after is the input for the command
		char **tokenizedBuffer = sys_alloc_mem(10);
		
		char* token = strtok(cmdBuffer, " ");
		
		int index = 0;
//		Change index number if you want to be able to input more than X number of words!
		for(index=0; index<5;index++){
			tokenizedBuffer[index] = token;
			token = strtok(NULL, " ");
	
			
		}

//		Comparing first token against all avaiable commands according to comhandsupport.h
		int i = -1;
		int k;
		int comLength = sizeof(commands)/sizeof(commands[0]);
		for(k=0; k<comLength; k++){
			if(strcmp(tokenizedBuffer[0], commands[k])==0){
				i = k+1;
			}
		}
		
		switch(i){
			case 1: version(); 						break;
			case 2: inputHelp(tokenizedBuffer[1]);	break;
			case 3: quit = shutDown();				break;
			case 4: getTime();						break;
			case 5: getDate();						break;
			case 6: setTime();						break;
			case 7: setDate();						break;
			case 8: create_pcb(tokenizedBuffer[1], atoi(tokenizedBuffer[2]), atoi(tokenizedBuffer[3]));	break;
			case 9: show_all();						break;
			case 10: show_blocked();				break;
			case 11: show_ready();					break;
			case 12: show_pcb(tokenizedBuffer[1]);  break;
			case 13: delete_pcb(tokenizedBuffer[1]);break;
			case 14: suspend_pcb(tokenizedBuffer[1]); break;
			case 15: resume_pcb(tokenizedBuffer[1]); break;
			//case 16: block_pcb(tokenizedBuffer[1]); break;
			//case 17: unblock_pcb(tokenizedBuffer[1]); break;
			case 16: set_pcb_priority(tokenizedBuffer[1], atoi(tokenizedBuffer[2]));break;
			//case 17: yield(); break;
			case 17: loadr3(); break;
			case 18: clear(); break;
			case 19: setAlarm(); break;
			case 20: infinite(); break;
			case 21: show_free_mem(); break;
			case 22: show_alloc_mem(); break;
			case 23: function();
			default: write_text_red("Not a valid command, type ");
					 write_text_yellow("'help' "); 
					 write_text_red("for options\n");
					 break;
		}
		if(quit == 0){
			sys_req(IDLE, DEFAULT_DEVICE, NULL, NULL);
		}
	}
//Remove and free all queues
//When comhand quits, the queues should be empty

	suspend_pcb("infinite_process");
	remove_all();
	sys_req(EXIT, DEFAULT_DEVICE, NULL, NULL);
	return -1;

}

void displayMenu(){
	//Displays the Menu at the beginning of the OS
	static int headerSize = 70;

	static char header1[70] = {"\033[1;32m _   _  ___  ___  _____    __    _  _____  __        __  ___\n"};
	static char header2[70] = {"| | | || __|| __||  _  |  |  \\  | ||  _  ||  \\      /  || __|\n"};
	static char header3[70] = {"| | | || |_ | |_ | |_| |  |   \\ | || |_| ||   \\    /   || |_ \n"};
	static char header4[70] = {"\033[1;34m| | | ||__ ||  _||  __ \\  | |\\ \\| ||  _  || |\\ \\  / /| ||  _|\n"};
	static char header5[70] = {"| |_| | _| || |_ | |  \\ \\ | | \\ \\ || | | || | \\ \\/ / | || |_ \n"};
	static char header6[70] = {"|_____||___||___||_|   \\_\\|_|  \\__||_| |_||_|  \\__/  |_||___|\033[0m\n\n"};
	sys_req(WRITE,DEFAULT_DEVICE,header1,&headerSize);
	sys_req(WRITE,DEFAULT_DEVICE,header2,&headerSize);
	sys_req(WRITE,DEFAULT_DEVICE,header3,&headerSize);
	sys_req(WRITE,DEFAULT_DEVICE,header4,&headerSize);
	sys_req(WRITE,DEFAULT_DEVICE,header5,&headerSize);
	sys_req(WRITE,DEFAULT_DEVICE,header6,&headerSize);

	static char init_text[150] = "\033[0;35mMPX-OS V2.70\033[0m\nType \033[1;33mhelp\033[0m for a list of commands...\n\n\n";
	static int init_size = 150;
	sys_req(WRITE, DEFAULT_DEVICE, init_text, &init_size);

}

void auto_complete(char partial_str[]){
	int i;
	int num_cmds = sizeof(commands);
	write_text(itoa(num_cmds));
	for(i = 0; i < num_cmds; i++){
		char cmd_str[30];
		strcpy(cmd_str, commands[i]);
		int k;
		int same = 1;
		int str_size = sizeof(partial_str);
		for(k = 0; k < str_size; i++){
			if(partial_str[k] != cmd_str[k]){
				same = 0;
			}
		}
		if(same == 1){
			serial_print(commands[i]);
		}else{
			serial_print("");
		}
	}
}





