#include <system.h>
#include <core/io.h>
#include <core/serial.h>
#include <string.h>
#include "../mpx_supt.h"
#include "comhand.h"
#include "time.h"
#include "date.h"
#include "comhandsupport.h"
#include "../R2/pcb.h"






void version(){
	int verSize = 30;
	char version[30] = "\033[1;32mVersion 2.4\033[0m\n\n";
	sys_req(WRITE,DEFAULT_DEVICE, version, &verSize);
}

void displayAllCommands(){
	int msg_size = 100;
	char message[100] = "Avaiable Commands are \n";
	sys_req(WRITE, DEFAULT_DEVICE, message, &msg_size);
	int k;
	int comLength = sizeof(commands)/sizeof(commands[0]);
	int one = 1;
	for(k=0; k<comLength; k++){
		int tempSize = 50;
		
		sys_req(WRITE,DEFAULT_DEVICE,commands[k],&tempSize);
		sys_req(WRITE,DEFAULT_DEVICE,"\n",&one);
	}
	sys_req(WRITE,DEFAULT_DEVICE,"\n",&one);
}

int inputHelp(char helpBuffer[]){
	int i = -1;
	int k;
	int comLength = sizeof(commands)/sizeof(commands[0]);
	for(k=0; k<comLength; k++){
		if(strcmp(helpBuffer, commands[k])==0){
			i = k+1;
		}
	}
	int msg_size = 300;
	char messageVersion[300] = "Version displays the version of MPX-OS currently running.\n"; 
	char messageHelp[300] = "Help displays necessary information on the function. \n";
	char messageShutdown[300] = "Shutdown will power off MPX-OS, you will first be asked for confirmation before powering off by selecting (1) to confirm, or (2) to cancel.\n";
	char messageDisplayTime[300] = "Displays the current time of day.\n";
	char messageDisplayDate[300] = "Displays the current date.\n";
	char messageSetTime[300] = "Prompts the user for an hour, minute, and second to change the MPX clock to.\n";
	char messageSetDate[300] = "Prompts the user for a month, day, and year to change the MPX date to.\n";	
	char messageCreatePCB[300] = "Creates a PCB.\n";	
	char messageShowAll[300] = "Shows all PCBs Created.\n";	
	char messageShowReady[300] = "Shows PCBs that are ready for execution,\n";
	char messageShowBlocked[300] = "Shows all the blocked PCBs.\n";
	char messageShowPCB[300] = "Shows a single, user chosen PCB.\n";
	char messageDeletePCB[300] = "Deletes a PCB from the appropriate queue and then free all associated memory.\n";
	char messageSuspendPCB[300] = "Places a PCB in the suspended state.\n";
	char messageResumePCB[300] = "Places a PCB in the not suspended state.\n";
	char messageBlockPCB[300] = "Sets a PCB's state to blocked.\n";
	char messageUnblockPCB[300] = "Sets a PCB's state to unblocked.\n";
	char messageSetPCBPriority[300] = "Sets a PCB's priority.\n";
	
		switch(i){
			case 1: sys_req(WRITE, DEFAULT_DEVICE, messageVersion, &msg_size);   break;

			case 2: sys_req(WRITE, DEFAULT_DEVICE, messageHelp, &msg_size); break;

			case 3: sys_req(WRITE, DEFAULT_DEVICE, messageShutdown, &msg_size); break;

			case 4:	sys_req(WRITE, DEFAULT_DEVICE, messageDisplayTime, &msg_size); break;

			case 5: sys_req(WRITE, DEFAULT_DEVICE, messageDisplayDate, &msg_size); break;

			case 6: sys_req(WRITE, DEFAULT_DEVICE, messageSetTime, &msg_size); break;

			case 7: sys_req(WRITE, DEFAULT_DEVICE, messageSetDate, &msg_size); break;

			case 8: sys_req(WRITE, DEFAULT_DEVICE, messageCreatePCB, &msg_size); break;

			case 9: sys_req(WRITE, DEFAULT_DEVICE, messageShowAll, &msg_size); break;

			case 10: sys_req(WRITE, DEFAULT_DEVICE, messageShowReady, &msg_size); break;

			case 11: sys_req(WRITE, DEFAULT_DEVICE, messageShowBlocked, &msg_size); break;

			case 12: sys_req(WRITE, DEFAULT_DEVICE, messageShowPCB, &msg_size); break;
			
			case 13: sys_req(WRITE, DEFAULT_DEVICE, messageDeletePCB, &msg_size); break;

			case 14: sys_req(WRITE, DEFAULT_DEVICE, messageSuspendPCB, &msg_size); break;

			case 15: sys_req(WRITE, DEFAULT_DEVICE, messageResumePCB, &msg_size); break;
		
			case 16: sys_req(WRITE, DEFAULT_DEVICE, messageBlockPCB, &msg_size); break;

			case 17: sys_req(WRITE, DEFAULT_DEVICE, messageUnblockPCB, &msg_size); break;

			case 18: sys_req(WRITE, DEFAULT_DEVICE, messageSetPCBPriority, &msg_size); break;

			default: displayAllCommands();
		}
		return 0;

}


int shutDown(){
	int promptInt = 50;
	char prompt[50] = "Are you sure? (\033[0;32mY\033[0m/\033[0;31mN\033[0m)\n";
	int ansInt = 2;
	char ans[2];
	memset(ans, '\0', 2);
	sys_req(WRITE, DEFAULT_DEVICE, prompt, &promptInt);
	sys_req(READ, DEFAULT_DEVICE, ans, &ansInt);
	
	if(strcmp(ans, "Y")==0){

		sys_req(WRITE, DEFAULT_DEVICE, "\033[1;33mMPX is shutting down...\033[0m\n", &promptInt);

		return 1;
	}else if(strcmp(ans, "N")==0){
		sys_req(WRITE, DEFAULT_DEVICE, "\033[1;33mShutdown canceled\033[0m\n", &promptInt);
		return 0;
	}else{
		int failedAbortSize = 50;
		char failedAbort[50] = "\n\033[1;31mNot valid command, aborting...\033[0m\n";
		sys_req(WRITE, DEFAULT_DEVICE, failedAbort, &failedAbortSize);
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
		//int size = 16;
		//sys_req(WRITE, DEFAULT_DEVICE, tokenizedBuffer[1], &size);
		
		int failSize = 100;
		char failure[100] = "\033[0;31mNot a recognized command:\033[0m type 'help' to see a list of commands.\n\n";
		switch(i){
			case 1: version(); 						break;
			case 2: inputHelp(tokenizedBuffer[1]);	break;
			case 3: quit = shutDown();				break;
			case 4: getTime();						break;
			case 5: getDate();						break;
			case 6: setTime();						break;
			case 7: setDate();						break;
			case 8: create_pcb();					break;
			case 9: show_all();						break;
			case 10: show_blocked();				break;
			case 11: show_ready();					break;
			case 12: show_pcb(tokenizedBuffer[1]);  break;
			case 13: delete_pcb(tokenizedBuffer[1]);break;
			case 14: suspend_pcb(tokenizedBuffer[1]); break;
			case 15: resume_pcb(tokenizedBuffer[1]); break;
			case 16: block_pcb(tokenizedBuffer[1]); break;
			case 17: unblock_pcb(tokenizedBuffer[1]); break;
			case 18: set_pcb_priority(tokenizedBuffer[1], atoi(tokenizedBuffer[2]));break;
			default: sys_req(WRITE,DEFAULT_DEVICE, failure, &failSize);
		}
	}
	return -1;

}

void displayMenu(){
	int headerSize = 70;
	char header1[70] = {"\033[1;32m _   _  ___  ___  _____    __    _  _____  __        __  ___\n"};
	char header2[70] = {"| | | || __|| __||  _  |  |  \\  | ||  _  ||  \\      /  || __|\n"};
	char header3[70] = {"| | | || |_ | |_ | |_| |  |   \\ | || |_| ||   \\    /   || |_ \n"};
	char header4[70] = {"| | | ||__ ||  _||  __ \\  | |\\ \\| ||  _  || |\\ \\  / /| ||  _|\n"};
	char header5[70] = {"| |_| | _| || |_ | |  \\ \\ | | \\ \\ || | | || | \\ \\/ / | || |_ \n"};
	char header6[70] = {"|_____||___||___||_|   \\_\\|_|  \\__||_| |_||_|  \\__/  |_||___|\033[0m\n\n"};
	sys_req(WRITE,DEFAULT_DEVICE,header1,&headerSize);
	sys_req(WRITE,DEFAULT_DEVICE,header2,&headerSize);
	sys_req(WRITE,DEFAULT_DEVICE,header3,&headerSize);
	sys_req(WRITE,DEFAULT_DEVICE,header4,&headerSize);
	sys_req(WRITE,DEFAULT_DEVICE,header5,&headerSize);
	sys_req(WRITE,DEFAULT_DEVICE,header6,&headerSize);
//	sys_req(WRITE,DEFAULT_DEVICE, menu, &menuSize);
	int k;
	int comLength = sizeof(commands)/sizeof(commands[0]);
	int one = 1;
	for(k=0; k<comLength; k++){
		int tempSize = 50;
		
		sys_req(WRITE,DEFAULT_DEVICE,commands[k],&tempSize);
		sys_req(WRITE,DEFAULT_DEVICE,"\n",&one);
	}
	sys_req(WRITE,DEFAULT_DEVICE,"\n",&one);

}

void comhandinitaliz(){
	displayMenu();
	comhand();
//	return -1;
}



