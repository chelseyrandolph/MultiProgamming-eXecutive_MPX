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
#include "../R3/Dispatcher.h"





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
	
	char *messageVersion= "\nVersion displays the version of MPX-OS currently running.\n"; 
	int versionSize = sizeof(messageVersion);

	char *messageHelp = "\nHelp displays necessary information on the function. \n";
	int helpSize = sizeof(messageHelp);

	char *messageShutdown = "\nShutdown will power off MPX-OS, you will first be asked for confirmation before powering off by selecting (1) to confirm, or (2) to cancel.\n";
	int shutdownSize = sizeof(messageShutdown);

	char *messageDisplayTime = "\nDisplays the current time of day.\n";
	int displayTimeSize = sizeof(messageDisplayTime);

	char *messageDisplayDate = "\nDisplays the current date.\n";
	int displayDateSize = sizeof(messageDisplayDate);

	char *messageSetTime = "\nPrompts the user for an hour, minute, and second to change the MPX clock to.\n";
	int setTimeSize = sizeof(messageSetTime);

	char *messageSetDate = "\nPrompts the user for a month, day, and year to change the MPX date to.\n";	
	int setDateSize = sizeof(messageSetDate);

	char *messageCreatePCB = "\nCreates a PCB.\n";
	int createPCBSize = sizeof(messageCreatePCB);
	
	char *messageShowAll = "\nShows all PCBs Created.\n";	
	int showAllSize = sizeof(messageShowAll);

	char *messageShowReady = "\nShows PCBs that are ready for execution,\n";
	int showReadySize = sizeof(messageShowReady);

	char *messageShowBlocked = "\nShows all the blocked PCBs.\n";
	int showBlockedSize = sizeof(messageShowBlocked);

	char *messageShowPCB = "\nShows a single, user chosen PCB.\n";
	int showPCBSize = sizeof(messageShowPCB);

	char *messageDeletePCB = "\nDeletes a PCB from the appropriate queue and then free all associated memory.\n";
	int deletePCBSize = sizeof(messageDeletePCB);

	char *messageSuspendPCB = "\nPlaces a PCB in the suspended state.\n";
	int suspendPCBSize = sizeof(messageSuspendPCB);

	char *messageResumePCB = "\nPlaces a PCB in the not suspended state.\n";
	int resumePCBSize = sizeof(messageResumePCB);

	char *messageBlockPCB = "\nSets a PCB's state to blocked.\n";
	int blockPCBSize = sizeof(messageBlockPCB);

	char *messageUnblockPCB = "\nSets a PCB's state to unblocked.\n";
	int unblockPCBSize = sizeof(messageUnblockPCB);

	char *messageSetPCBPriority = "\nSets a PCB's priority.\n";
	int setPCBPrioritySize = sizeof(messageSetPCBPriority);

	char *loadr3msg = "\nLoads test processes.\n";
	int loadr3Size = sizeof(loadr3msg);
	
		switch(i){
			case 1: sys_req(WRITE, DEFAULT_DEVICE, helpcommands[i-1], &tempSize);
					sys_req(WRITE, DEFAULT_DEVICE, messageVersion, &versionSize);
					break;

			case 2: sys_req(WRITE, DEFAULT_DEVICE, helpcommands[i-1], &tempSize);  
					sys_req(WRITE, DEFAULT_DEVICE, messageHelp, &helpSize);
					break;

			case 3: sys_req(WRITE, DEFAULT_DEVICE, helpcommands[i-1], &tempSize);  
					sys_req(WRITE, DEFAULT_DEVICE, messageShutdown, &shutdownSize); 
					break;

			case 4:	sys_req(WRITE, DEFAULT_DEVICE, helpcommands[i], &tempSize); 
					sys_req(WRITE, DEFAULT_DEVICE, messageDisplayTime, &displayTimeSize);
					break;

			case 5: sys_req(WRITE, DEFAULT_DEVICE, helpcommands[i-1], &tempSize);
					sys_req(WRITE, DEFAULT_DEVICE, messageDisplayDate, &displayDateSize);  
					break;

			case 6: sys_req(WRITE, DEFAULT_DEVICE, helpcommands[i-1], &tempSize); 
					sys_req(WRITE, DEFAULT_DEVICE, messageSetTime, &setTimeSize); 
					break;

			case 7: sys_req(WRITE, DEFAULT_DEVICE, helpcommands[i-1], &tempSize);
					sys_req(WRITE, DEFAULT_DEVICE, messageSetDate, &setDateSize);
					break;

			case 8: sys_req(WRITE, DEFAULT_DEVICE, helpcommands[i-1], &tempSize);  
					sys_req(WRITE, DEFAULT_DEVICE, messageCreatePCB, &createPCBSize); 
					break;

			case 9: sys_req(WRITE, DEFAULT_DEVICE, helpcommands[i-1], &tempSize);
					sys_req(WRITE, DEFAULT_DEVICE, messageShowAll, &showAllSize);   
					break;

			case 10: sys_req(WRITE, DEFAULT_DEVICE, helpcommands[i-1], &tempSize); 
					 sys_req(WRITE, DEFAULT_DEVICE, messageShowReady, &showReadySize); 
					 break;

			case 11: sys_req(WRITE, DEFAULT_DEVICE, helpcommands[i-1], &tempSize); 
					 sys_req(WRITE, DEFAULT_DEVICE, messageShowBlocked, &showBlockedSize); 
					 break;

			case 12: sys_req(WRITE, DEFAULT_DEVICE, helpcommands[i-1], &tempSize); 
					 sys_req(WRITE, DEFAULT_DEVICE, messageShowPCB, &showPCBSize); 
					 break;
			
			case 13: sys_req(WRITE, DEFAULT_DEVICE, helpcommands[i-1], &tempSize); 
					 sys_req(WRITE, DEFAULT_DEVICE, messageDeletePCB, &deletePCBSize);
					 break;

			case 14: sys_req(WRITE, DEFAULT_DEVICE, helpcommands[i-1], &tempSize); 
					 sys_req(WRITE, DEFAULT_DEVICE, messageSuspendPCB, &suspendPCBSize); 
					 break;

			case 15: sys_req(WRITE, DEFAULT_DEVICE, helpcommands[i-1], &tempSize);
					 sys_req(WRITE, DEFAULT_DEVICE, messageResumePCB, &resumePCBSize); 
					 break;
		
			case 16: sys_req(WRITE, DEFAULT_DEVICE, helpcommands[i-1], &tempSize); 
					 sys_req(WRITE, DEFAULT_DEVICE, messageBlockPCB, &blockPCBSize); 
					 break;

			case 17: sys_req(WRITE, DEFAULT_DEVICE, helpcommands[i-1], &tempSize); 
					 sys_req(WRITE, DEFAULT_DEVICE, messageUnblockPCB, &unblockPCBSize); 
					 break;

			case 18: sys_req(WRITE, DEFAULT_DEVICE, helpcommands[i-1], &tempSize); 
					 sys_req(WRITE, DEFAULT_DEVICE, messageSetPCBPriority, &setPCBPrioritySize); 
					 break;

			case 19: sys_req(WRITE, DEFAULT_DEVICE, helpcommands[i-1], &tempSize); 
					 sys_req(WRITE, DEFAULT_DEVICE, loadr3msg, &loadr3Size); 
					 break;

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

		//sys_req(WRITE, DEFAULT_DEVICE, "\033[1;33mMPX is shutting down...\033[0m\n", &promptInt);
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
			case 8: create_pcb(tokenizedBuffer[1], atoi(tokenizedBuffer[2]), atoi(tokenizedBuffer[3]));	break;
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
			case 19: loadr3(); break;
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
	char header4[70] = {"\033[1;34m| | | ||__ ||  _||  __ \\  | |\\ \\| ||  _  || |\\ \\  / /| ||  _|\n"};
	char header5[70] = {"| |_| | _| || |_ | |  \\ \\ | | \\ \\ || | | || | \\ \\/ / | || |_ \n"};
	char header6[70] = {"|_____||___||___||_|   \\_\\|_|  \\__||_| |_||_|  \\__/  |_||___|\033[0m\n\n"};
	sys_req(WRITE,DEFAULT_DEVICE,header1,&headerSize);
	sys_req(WRITE,DEFAULT_DEVICE,header2,&headerSize);
	sys_req(WRITE,DEFAULT_DEVICE,header3,&headerSize);
	sys_req(WRITE,DEFAULT_DEVICE,header4,&headerSize);
	sys_req(WRITE,DEFAULT_DEVICE,header5,&headerSize);
	sys_req(WRITE,DEFAULT_DEVICE,header6,&headerSize);
//	sys_req(WRITE,DEFAULT_DEVICE, menu, &menuSize);
	/*int k;
	int comLength = sizeof(commands)/sizeof(commands[0]);
	int one = 1;
	for(k=0; k<comLength; k++){
		int tempSize = 50;
		
		sys_req(WRITE,DEFAULT_DEVICE,commands[k],&tempSize);
		sys_req(WRITE,DEFAULT_DEVICE,"\n",&one);
	}
	sys_req(WRITE,DEFAULT_DEVICE,"\n",&one);
	*/
}

void comhandinitaliz(){
	displayMenu();
	char *init_text = "\033[0;35mMPX-OS V2.70\033[0m\nType \033[1;33mhelp\033[0m for a list of commands...\n\n\n";
	int init_size = sizeof(init_text);
	sys_req(WRITE, DEFAULT_DEVICE, init_text, &init_size);
	comhand();
//	return -1;
}



