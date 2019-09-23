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

/*
	Help works the same was as comhand() but exits after an option is selected
*/
int ourHelp(){
	int menuSize = 250;
	char menu[250] = {"\nPress Corresponding number to get information on commands \n 1. Getting the Version \n 2. Shutting down MPX-OS \n 3. Getting the time \n 4. Getting the Date \n 5. Setting the Time \n 6. Setting the Date \n 7. Exit Help\n\n"};
	sys_req(WRITE,DEFAULT_DEVICE, menu, &menuSize);
	char cmdBuffer[2];
	memset(cmdBuffer, '\0', 100);
	int bufferSize = 20;
	sys_req(READ,DEFAULT_DEVICE,cmdBuffer,&bufferSize);
	int msg_size = 300;
	int i = atoi(cmdBuffer); 
	if(i==1){
		char message[300] = "Version displays the version of MPX-OS currently running.\n"; 	
		sys_req(WRITE, DEFAULT_DEVICE, message, &msg_size);
		return 0;
	}
	else if(i==2){
		char message[300] = "Shutdown will power off MPX-OS, you will first be asked for confirmation before powering off by selecting (1) to confirm, or (2) to cancel.\n";	
		sys_req(WRITE, DEFAULT_DEVICE, message, &msg_size);
		return 0;
	}
	else if(i==3){
		char message[300] = "Displays the current time of day.\n";		
		sys_req(WRITE, DEFAULT_DEVICE, message, &msg_size);
		return 0;
	}
	else if(i==4){
		char message[300] = "Displays the current date.\n";	
		sys_req(WRITE, DEFAULT_DEVICE, message, &msg_size);
		return 0;
	}
	else if(i==5){
		char message[300] = "Prompts the user for an hour, minute, and second to change the MPX clock to.\n";	
		sys_req(WRITE, DEFAULT_DEVICE, message, &msg_size);
		return 0;
	}
	else if(i==6){
		char message[300] = "Prompts the user for a month, day, and year to change the MPX date to.\n";	
		sys_req(WRITE, DEFAULT_DEVICE, message, &msg_size);
		return 0;
	}
	else if(i==7){
		return 0;
	}
	else{
		int failedAbortSize = 50;
		char failedAbort[50] = "\nNot valid command, aborting to main menu.\n";
		sys_req(WRITE, DEFAULT_DEVICE, failedAbort, &failedAbortSize);
		return 0;
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
	char cmdBuffer[100];
	int bufferSize;
	int quit = 0;
	
	while(!quit){

		

		//get a command
		memset(cmdBuffer, '\0', 100);
		bufferSize = 99;
		sys_req(READ,DEFAULT_DEVICE,cmdBuffer,&bufferSize);
		/*
		WORK IN PROGRESS TO TOKENIZE THE INPUT COMMAND
		char *tokenizedBuffer[5];
		int temp=0;
		for(;temp<2;temp++){
			char *cmdBufferPtr = strtok(cmdBuffer, " ");

			*tokenizedBuffer[temp] = strcpy(*tokenizedBuffer[temp],*cmdBufferPtr);
		}
//		char *cmdBufferPtr = strtok(cmdBuffer, " ");
	//	cmdBufferPtr = strtok(cmdBuffer, " ");

	
		sys_req(WRITE,DEFAULT_DEVICE,tokenizedBuffer[1], &bufferSize);*/
		
		int i = -1;
		int k;
		int comLength = sizeof(commands)/sizeof(commands[0]);
		for(k=0; k<comLength; k++){
			if(strcmp(cmdBuffer, commands[k])==0){
				i = k+1;
			}
		}
	
		int failSize = 100;
		char failure[100] = "\033[0;31mNot a recognized command:\033[0m type 'help' to see a list of commands.\n\n";
		switch(i){
			case 1: version(); 					break;
			case 2: ourHelp();					break;
			case 3: quit = shutDown();				break;
			case 4: getTime();					break;
			case 5: getDate();					break;
			case 6: setTime();					break;
			case 7: setDate();					break;
			case 8: create_pcb();				break;
			case 9: show_all();				break;
			case 10: show_blocked();			break;
			case 11: show_ready();				break;
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



