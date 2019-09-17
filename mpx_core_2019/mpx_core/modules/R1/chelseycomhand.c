#include <system.h>
#include <core/io.h>
#include <core/serial.h>
#include <string.h>
#include "../mpx_supt.h"
#include "comhand.h"
#include "time.h"
#include "date.h"






void version(){
	int verSize = 20;
	char version[20] = "Version 1.2\n";
	sys_req(WRITE,DEFAULT_DEVICE, version, &verSize);
}

/*
	Help works the same was as comhand() but exits after an option is selected
*/
int ourHelp(char *cmd){
	int msg_size = 300;
	int menuSize = 250;
	if(strcmp(cmd, "version") == 0){
		char message[300] = "Version displays the version of MPX-OS currently running.\n"; 	
		sys_req(WRITE, DEFAULT_DEVICE, message, &msg_size);
		return 0;
	}
	else if(strcmp(cmd, "shutdown") == 0){
		char message[300] = "Shutdown will power off MPX-OS, you will first be asked for confirmation before powering off by selecting (1) to confirm, or (2) to cancel.\n";	
		sys_req(WRITE, DEFAULT_DEVICE, message, &msg_size);
		return 0;
	}
	else if(strcmp(cmd, "getTime") == 0){
		char message[300] = "Displays the current time of day.\n";		
		sys_req(WRITE, DEFAULT_DEVICE, message, &msg_size);
		return 0;
	}
	else if(strcmp(cmd, "getDate") == 0){
		char message[300] = "Displays the current date.\n";	
		sys_req(WRITE, DEFAULT_DEVICE, message, &msg_size);
		return 0;
	}
	else if(strcmp(cmd, "setTime") == 0){
		char message[300] = "Prompts the user for an hour, minute, and second to change the MPX clock to.\n";	
		sys_req(WRITE, DEFAULT_DEVICE, message, &msg_size);
		return 0;
	}
	else if(strcmp(cmd, "setDate") == 0){
		char message[300] = "Prompts the user for a month, day, and year to change the MPX date to.\n";	
		sys_req(WRITE, DEFAULT_DEVICE, message, &msg_size);
		return 0;
	}
	else if(strcmp(cmd, "all") == 0){
		char menu[250] = {"\nWelcome to MPX Help: \nversion \nshutdown \ngetTime \ngetDate \nsetTime \nsetDate \n\n"};
		sys_req(WRITE, DEFAULT_DEVICE, menu, &menuSize);
		comhand();
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
	char prompt[50] = "Are you sure? \n 1. Yes \n 2. No\n";
	int ansInt;
	char ans[2];
	memset(ans, '\0', 2);
	sys_req(WRITE, DEFAULT_DEVICE, prompt, &promptInt);
	sys_req(READ, DEFAULT_DEVICE, ans, &ansInt);
	//sys_req(WRITE, DEFAULT_DEVICE, ans, &ansInt);
	int i = atoi(ans);
	
	if(i==1){

		serial_print("MPX is shuting down");

		return 1;
	}else if(i==2){
		serial_print("Shutdown process is canceled by the user.");
		return 0;
	}else{
		int failedAbortSize = 50;
		char failedAbort[50] = "\nNot valid command, aborting to main menu\n";
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
		int failSize = 100;
		char failure[100] = "Not a valid command, please enter a valid number.\n";
			
		int i = 0;
		int j = 0;
		char tmpBuf[5];
		char helpCmd[100];
		tmpBuf[5] = '\0';
		for(i=0; i<4; i++){
			tmpBuf[i] = cmdBuffer[i];
		}
		memset(helpCmd, '\0', 100);
		for(j = 0; j < strlen(cmdBuffer) -5; j++){
			helpCmd[j] = cmdBuffer[j+5];
		}
		if(strcmp(cmdBuffer, "version") == 0){
			version();
		}else if(strcmp(tmpBuf, "help") == 0){
			ourHelp(helpCmd);
		}else if(strcmp(cmdBuffer, "shutdown") == 0){
			quit = shutDown();
		}else if(strcmp(cmdBuffer, "getTime") == 0){
			getTime();
		}else if(strcmp(cmdBuffer, "getDate") == 0){
			getDate();
		}else if(strcmp(cmdBuffer, "setTime") == 0){
			setTime();
		}else if(strcmp(cmdBuffer, "setDate") == 0){
			setDate();
		}else{
			sys_req(WRITE,DEFAULT_DEVICE, failure, &failSize);
		}

	}
	return -1;

}



