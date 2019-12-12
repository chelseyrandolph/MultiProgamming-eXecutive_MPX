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



char history[100][100];
int historySize = 0;

void version(){			//Prints the current version
	int v = 5;
	printf("MPX-OS Version %d\n", v);
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

			/*case 8: sys_req(WRITE, DEFAULT_DEVICE, helpcommands[i-1], &tempSize);  
					write_text("\nCreates a PCB.\n\n"); 
					break;
			*/
			case 8: sys_req(WRITE, DEFAULT_DEVICE, helpcommands[i-1], &tempSize);
					write_text("\nShows all PCBs Created.\n\n");   
					break;

			case 9: sys_req(WRITE, DEFAULT_DEVICE, helpcommands[i-1], &tempSize); 
					 write_text("\nShows all the blocked PCBs.\n\n"); 
					 break;

			case 10: sys_req(WRITE, DEFAULT_DEVICE, helpcommands[i-1], &tempSize); 
					 write_text("\nShows PCBs that are ready for execution.\n\n"); 
					 break;

			case 11: sys_req(WRITE, DEFAULT_DEVICE, helpcommands[i-1], &tempSize); 
					 write_text("\nShows a single, user chosen PCB.\n\n"); 
					 break;
	
			case 12: sys_req(WRITE, DEFAULT_DEVICE, helpcommands[i-1], &tempSize); 
					 write_text("\nDeletes a PCB from the appropriate queue and then free all associated memory.\n\n");
					 break;
			
			case 13: sys_req(WRITE, DEFAULT_DEVICE, helpcommands[i-1], &tempSize); 
					 write_text("\nPlaces a PCB in the suspended state.\n\n"); 
					 break;

			case 14: sys_req(WRITE, DEFAULT_DEVICE, helpcommands[i-1], &tempSize);
					 write_text("\nPlaces a PCB in the not suspended state.\n\n"); 
					 break;
			
			case 15: sys_req(WRITE, DEFAULT_DEVICE, helpcommands[i-1], &tempSize); 
					 write_text("\nSets a PCB's state to blocked.\n\n"); 
					 break;

			case 16: sys_req(WRITE, DEFAULT_DEVICE, helpcommands[i-1], &tempSize); 
					 write_text("\nSets a PCB's state to unblocked.\n\n"); 
					 break;
			

			case 17: sys_req(WRITE, DEFAULT_DEVICE, helpcommands[i-1], &tempSize); 
					 write_text("\nSets a PCB's priority.\n\n"); 
					 break;
/*
			case 17: sys_req(WRITE, DEFAULT_DEVICE, helpcommands[i-1], &tempSize);
					 write_text("\nCauses comhand to yield to other processes.\n\n");
					 break;
*/
			case 18: sys_req(WRITE, DEFAULT_DEVICE, helpcommands[i-1], &tempSize); 
					 write_text("\nLoads test processes.\n\n"); 
					 break;

			case 19: sys_req(WRITE, DEFAULT_DEVICE, helpcommands[i-1], &tempSize); 
					 write_text("\nThis clears the terminal.\n\n"); 
					 break;

			case 20: sys_req(WRITE, DEFAULT_DEVICE, helpcommands[i-1], &tempSize); 
					 write_text("\nThis creates an alarm.\n\n");
					 break;

			case 21: sys_req(WRITE, DEFAULT_DEVICE, helpcommands[i-1], &tempSize); 
					 write_text("\nThis creates the infinite process.\n\n");
					 break;

			case 22: sys_req(WRITE, DEFAULT_DEVICE, helpcommands[i-1], &tempSize); 
					 write_text("\nThis shows the free memory blocks.\n\n");
					 break;

			case 23: sys_req(WRITE, DEFAULT_DEVICE, helpcommands[i-1], &tempSize); 
					 write_text("\nThis shows the allocated memory blocks.\n\n");
					 break;
			
			case 24: sys_req(WRITE, DEFAULT_DEVICE, helpcommands[i-1], &tempSize); 
					 write_text("\nThis is a test function for R5.\n\n");
					 break;

			case 25: sys_req(WRITE, DEFAULT_DEVICE, helpcommands[i-1], &tempSize); 
					 write_text("\nThis shows the current commands in history.\n\n");
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

int getHistorySize(){
	return historySize;
}

char* getCommand(int index){
	return history[index];
}

void addToCmdHistory(char string[100]){
	strcpy(history[historySize], string);
	historySize++;
}

int comhand(){
	int quit = 0;
	while(!quit){
		char cmdBuffer[100]; //= (char*)alloc_mem(100);
		int bufferSize;

	
//		Get a command
		memset(cmdBuffer, '\0', 100);
		bufferSize = 99;
		sys_req(READ,DEFAULT_DEVICE,cmdBuffer,&bufferSize);
//		Tokenize buffer. 
//		This set of instructions will break the buffer on white space and put it inside a *array
//		0 index is always the command wihle everything after is the input for the command
		char tokenizedBuffer[100][100]; 
		// = (char**)sys_alloc_mem(10);

		char token[100];
		strcpy(token, strtok(cmdBuffer, " "));
		
		int index = 0;
//		Change index number if you want to be able to input more than X number of words!
		for(index=0; index<5;index++){
			strcpy(tokenizedBuffer[index], token);
			strcpy(token, strtok(NULL, " "));
	
			
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
			//case 8: create_pcb(tokenizedBuffer[1], atoi(tokenizedBuffer[2]), atoi(tokenizedBuffer[3]));	break;
			case 8: show_all();						break;
			case 9: show_blocked();				break;
			case 10: show_ready();					break;
			case 11: show_pcb(tokenizedBuffer[1]);  break;
			case 12: delete_pcb(tokenizedBuffer[1]);break;
			case 13: suspend_pcb(tokenizedBuffer[1]); break;
			case 14: resume_pcb(tokenizedBuffer[1]); break;
			case 15: block_pcb(tokenizedBuffer[1]); break;
			case 16: unblock_pcb(tokenizedBuffer[1]); break;
			case 17: set_pcb_priority(tokenizedBuffer[1], atoi(tokenizedBuffer[2]));break;
			//case 17: yield(); break;
			case 18: loadr3(); break;
			case 19: clear(); break;
			case 20: setAlarm(); break;
			case 21: loadProcess("infinite_process", 1, 0, &infinite); break;
			case 22: show_free_mem(); break;
			case 23: show_alloc_mem(); break;
			case 24: function(); break;
			case 25: showHistory();  break;
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
	if(find_pcb("infinite_process") != NULL){
		suspend_pcb("infinite_process");
	}

	remove_all();
	sys_req(EXIT, DEFAULT_DEVICE, NULL, NULL);
	return -1;

}

void displayMenu(){
	//Displays the Menu at the beginning of the OS
	static int headerSize = 70;

	static char header1[70] = {"\033[1;32m _   _  ___  ___  _____    __    _  _____  __        __  ___\n"};
	static char header2[70] = {"| | | || __|| __||  _  |  |  \\  | ||  _  ||  \\      /  || __|\n"};
	static char header3[70] = {"\033[1;31m| | | || |_ | |_ | |_| |  |   \\ | || |_| ||   \\    /   || |_ \n"};
	static char header4[70] = {"\033[1;34m| | | ||__ ||  _||  __ \\  | |\\ \\| ||  _  || |\\ \\  / /| ||  _|\n"};
	static char header5[70] = {"| |_| | _| || |_ | |  \\ \\ | | \\ \\ || | | || | \\ \\/ / | || |_ \n"};
	static char header6[76] = {"\033[1;35m|_____||___||___||_|   \\_\\|_|  \\__||_| |_||_|  \\__/  |_||___|\033[0m\n\n"};
	sys_req(WRITE,DEFAULT_DEVICE,header1,&headerSize);
	sys_req(WRITE,DEFAULT_DEVICE,header2,&headerSize);
	sys_req(WRITE,DEFAULT_DEVICE,header3,&headerSize);
	sys_req(WRITE,DEFAULT_DEVICE,header4,&headerSize);
	sys_req(WRITE,DEFAULT_DEVICE,header5,&headerSize);
	sys_req(WRITE,DEFAULT_DEVICE,header6,&headerSize);

	static char init_text[150] = "\033[0;35mMPX-OS Version 5.0\033[0m\nType \033[1;33mhelp\033[0m for a list of commands...\n\n\n";
	static int init_size = 150;
	sys_req(WRITE, DEFAULT_DEVICE, init_text, &init_size);

}

char* substring(char *source, int start, int end){
	int length = end - start;
	char *dest;
	int i = start;
	for(i = start; i < end && (*source != '\0'); i++){
		*dest = *(source + i);
		dest++;
	}
	*dest = '\0';
	return dest - length;
}

char *cmd_str = "";
char* auto_complete(char partial_str[]){
	int i;
	char *sub;
	int numOfMatches = 0;
	char matches[25][30];
	int index = -1;
	for(i = 0; i < 25; i++){
		int end = strlen(partial_str);
		strcpy(cmd_str, commands[i]);
		sub = substring(cmd_str, 0, end);
		if(strcmp(partial_str, sub) == 0){
			strcpy(matches[numOfMatches], commands[i]);
			numOfMatches += 1;
			index = i;
		}else{
			continue;
		}
	}
	if(numOfMatches > 1){
		for(i = 0; i < numOfMatches; i ++){
			write_text_yellow(matches[i]);
			write_text("\n");
		}
	} 
	if(numOfMatches == 1){
		return commands[index];
	}
	return partial_str;
}


void showHistory(){
	int i;
	for(i = 0; i < historySize; i++){
		write_text(history[i]);
		write_text("\n");
	}
}





