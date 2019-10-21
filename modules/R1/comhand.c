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





void version(){			//Prints the current version
	char version[20] = "MPX-OS Version 2.70\n";
	write_text_bold_green(version);
}

void displayAllCommands(){
	int msg_size = 100;
	char message[100] = "\033[1;34mAvaiable Commands are\033[0m \n";
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
	int size = 10;
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
	int size = 200;
	char messageVersion[200] = "\nVersion displays the version of MPX-OS currently running.\n"; 

	char messageHelp[200] = "\nHelp displays necessary information on the function. \n";

	char messageShutdown[200] = "\nShutdown will power off MPX-OS, you will first be asked for confirmation before powering off by selecting (1) to confirm, or (2) to cancel.\n";

	char messageDisplayTime[200] = "\nDisplays the current time of day.\n";

	char messageDisplayDate[200] = "\nDisplays the current date.\n";

	char messageSetTime[200] = "\nPrompts the user for an hour, minute, and second to change the MPX clock to.\n";

	char messageSetDate[200] = "\nPrompts the user for a month, day, and year to change the MPX date to.\n";

/* Temporary command from R2.
	char messageCreatePCB[200] = "\nCreates a PCB.\n";
*/	
	char messageShowAll[200] = "\nShows all PCBs Created.\n";

	char messageShowReady[200] = "\nShows PCBs that are ready for execution,\n";

	char messageShowBlocked[200] = "\nShows all the blocked PCBs.\n";

	char messageShowPCB[200] = "\nShows a single, user chosen PCB.\n";

	char messageDeletePCB[200] = "\nDeletes a PCB from the appropriate queue and then free all associated memory.\n";

	char messageSuspendPCB[200] = "\nPlaces a PCB in the suspended state.\n";

	char messageResumePCB[200] = "\nPlaces a PCB in the not suspended state.\n";

/* Temporary commands from R2.
	char messageBlockPCB[200] = "\nSets a PCB's state to blocked.\n";

	char messageUnblockPCB[200] = "\nSets a PCB's state to unblocked.\n";
*/

	char messageSetPCBPriority[200] = "\nSets a PCB's priority.\n";

	/*char messageYield[200] = "\nCauses comhand to yield to other processes.\n";
	*/
	char loadr3msg[200] = "\nLoads test processes.\n";

	char clearMsg[200] = "\nClears the terminal.\n";
	
		switch(i){
			case 1: sys_req(WRITE, DEFAULT_DEVICE, helpcommands[i-1], &tempSize);
					sys_req(WRITE, DEFAULT_DEVICE, messageVersion, &size);
					break;

			case 2: sys_req(WRITE, DEFAULT_DEVICE, helpcommands[i-1], &tempSize);  
					sys_req(WRITE, DEFAULT_DEVICE, messageHelp, &size);
					break;

			case 3: sys_req(WRITE, DEFAULT_DEVICE, helpcommands[i-1], &tempSize);  
					sys_req(WRITE, DEFAULT_DEVICE, messageShutdown, &size); 
					break;

			case 4:	sys_req(WRITE, DEFAULT_DEVICE, helpcommands[i], &tempSize); 
					sys_req(WRITE, DEFAULT_DEVICE, messageDisplayTime, &size);
					break;

			case 5: sys_req(WRITE, DEFAULT_DEVICE, helpcommands[i-1], &tempSize);
					sys_req(WRITE, DEFAULT_DEVICE, messageDisplayDate, &size);  
					break;

			case 6: sys_req(WRITE, DEFAULT_DEVICE, helpcommands[i-1], &tempSize); 
					sys_req(WRITE, DEFAULT_DEVICE, messageSetTime, &size); 
					break;

			case 7: sys_req(WRITE, DEFAULT_DEVICE, helpcommands[i-1], &tempSize);
					sys_req(WRITE, DEFAULT_DEVICE, messageSetDate, &size);
					break;

			/*case 8: sys_req(WRITE, DEFAULT_DEVICE, helpcommands[i-1], &tempSize);  
					sys_req(WRITE, DEFAULT_DEVICE, messageCreatePCB, &size); 
					break;
			*/
			case 8: sys_req(WRITE, DEFAULT_DEVICE, helpcommands[i-1], &tempSize);
					sys_req(WRITE, DEFAULT_DEVICE, messageShowAll, &size);   
					break;

			case 9: sys_req(WRITE, DEFAULT_DEVICE, helpcommands[i-1], &tempSize); 
					 sys_req(WRITE, DEFAULT_DEVICE, messageShowReady, &size); 
					 break;

			case 10: sys_req(WRITE, DEFAULT_DEVICE, helpcommands[i-1], &tempSize); 
					 sys_req(WRITE, DEFAULT_DEVICE, messageShowBlocked, &size); 
					 break;

			case 11: sys_req(WRITE, DEFAULT_DEVICE, helpcommands[i-1], &tempSize); 
					 sys_req(WRITE, DEFAULT_DEVICE, messageShowPCB, &size); 
					 break;
			
			case 12: sys_req(WRITE, DEFAULT_DEVICE, helpcommands[i-1], &tempSize); 
					 sys_req(WRITE, DEFAULT_DEVICE, messageDeletePCB, &size);
					 break;
			
			case 13: sys_req(WRITE, DEFAULT_DEVICE, helpcommands[i-1], &tempSize); 
					 sys_req(WRITE, DEFAULT_DEVICE, messageSuspendPCB, &size); 
					 break;

			case 14: sys_req(WRITE, DEFAULT_DEVICE, helpcommands[i-1], &tempSize);
					 sys_req(WRITE, DEFAULT_DEVICE, messageResumePCB, &size); 
					 break;
			/*
			case 16: sys_req(WRITE, DEFAULT_DEVICE, helpcommands[i-1], &tempSize); 
					 sys_req(WRITE, DEFAULT_DEVICE, messageBlockPCB, &size); 
					 break;

			case 17: sys_req(WRITE, DEFAULT_DEVICE, helpcommands[i-1], &tempSize); 
					 sys_req(WRITE, DEFAULT_DEVICE, messageUnblockPCB, &size); 
					 break;
			*/

			case 15: sys_req(WRITE, DEFAULT_DEVICE, helpcommands[i-1], &tempSize); 
					 sys_req(WRITE, DEFAULT_DEVICE, messageSetPCBPriority, &size); 
					 break;
			/*
			case 15: sys_req(WRITE, DEFAULT_DEVICE, helpcommands[i-1], &tempSize);
					 sys_req(WRITE, DEFAULT_DEVICE, messageYield, &size);
					 break;
			*/
			case 16: sys_req(WRITE, DEFAULT_DEVICE, helpcommands[i-1], &tempSize); 
					 sys_req(WRITE, DEFAULT_DEVICE, loadr3msg, &size); 
					 break;

			case 17: sys_req(WRITE, DEFAULT_DEVICE, helpcommands[i-1], &tempSize); 
					 sys_req(WRITE, DEFAULT_DEVICE, clearMsg, &size); 
					 break;

			default: displayAllCommands();
		}
		
		return 0;

}




int shutDown(){
	int size = 100;
	char prompt[100] = "Are you sure? (\033[0;32mY\033[0m/\033[0;31mN\033[0m)\n";
	int ansInt = 2;
	char ans[2];
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



int comhand(){
	int quit = 0;		
	char *cmdBuffer = sys_alloc_mem(100);
	memset(cmdBuffer, '\0', 100);
	int bufferSize;
    
//	Tokenize buffer. 
//	This set of instructions will break the buffer on white space and put it inside a *array
//	0 index is always the command wihle everything after is the input for the command
	char **tokenizedBuffer = sys_alloc_mem(10);


	while(!quit){
		//Get a command
		memset(cmdBuffer, '\0', 100);
	    bufferSize = 99;
		sys_req(READ,DEFAULT_DEVICE,cmdBuffer,&bufferSize);

		char* token = strtok(cmdBuffer, " ");
		
		int index = 0;
//		Change index number if you want to be able to input more than X number of words!
		for(index=0; index < 5; index++){
			tokenizedBuffer[index] = token;
			token = strtok(NULL, " ");		
		}
//		Comparing first token against all avaiable commands according to comhandsupport.h
		int i = -1;
		int k;
		int comLength = sizeof(commands)/sizeof(commands[0]);
		for(k = 0; k < comLength; k++){
			if(strcmp(tokenizedBuffer[0], commands[k])==0){
				i = k+1;
			}
		}
		
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
			//case 8: create_pcb(tokenizedBuffer[1], atoi(tokenizedBuffer[2]), atoi(tokenizedBuffer[3]));	break;
			case 8: show_all();						break;
			case 9: show_blocked();				break;
			case 10: show_ready();					break;
			case 11: show_pcb(tokenizedBuffer[1]);  break;
			case 12: delete_pcb(tokenizedBuffer[1]);break;
			case 13: suspend_pcb(tokenizedBuffer[1]); break;
			case 14: resume_pcb(tokenizedBuffer[1]); break;
			//case 16: block_pcb(tokenizedBuffer[1]); break;
			//case 17: unblock_pcb(tokenizedBuffer[1]); break;
			case 15: set_pcb_priority(tokenizedBuffer[1], atoi(tokenizedBuffer[2]));break;
			//case 19: yield(); break;
			case 16: loadr3(); break;
			case 17: clear(); break;
			default: sys_req(WRITE,DEFAULT_DEVICE, failure, &failSize);
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
	int headerSize = 70;

	char header1[70] = {"\033[1;32m _   _  ___  ___  _____    __    _  _____  __        __  ___\n"};
	char header2[70] = {"| | | || __|| __||  _  |  |  \\  | ||  _  ||  \\      /  || __|\n"};
	char header3[70] = {"| | | || |_ | |_ | |_| |  |   \\ | || |_| ||   \\    /   || |_ \n"};
	char header4[70] = {"\033[1;34m| | | ||__ ||  _||  __ \\  | |\\ \\| ||  _  || |\\ \\  / /| ||  _|\n"};
	char header5[70] = {"| |_| | _| || |_ | |  \\ \\ | | \\ \\ || | | || | \\ \\/ / | || |_ \n"};
	char header6[70] = {"|_____||___||___||_|   \\_\\|_|  \\__||_| |_||_|  \\__/  |_||___|\033[0m\n\n"};
	sys_req(WRITE,DEFAULT_DEVICE,header1,&headerSize);
	sys_req(WRITE,DEFAULT_DEVICE,header2,&headerSize);
	sys_req(WRITE,DEFAULT_DEVICE,header3,&headerSize);
	sys_req(WRITE,DEFAULT_DEVICE,header4,&headerSize);
	sys_req(WRITE,DEFAULT_DEVICE,header5,&headerSize);
	sys_req(WRITE,DEFAULT_DEVICE,header6,&headerSize);

	char init_text[150] = "\033[0;35mMPX-OS V2.70\033[0m\nType \033[1;33mhelp\033[0m for a list of commands...\n\n\n";
	int init_size = 150;
	sys_req(WRITE, DEFAULT_DEVICE, init_text, &init_size);

}





