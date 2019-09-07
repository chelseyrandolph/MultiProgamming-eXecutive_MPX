#include <system.h>
#include <core/io.h>
#include <core/serial.h>
#include <string.h>
#include "mpx_supt.h"
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
int ourHelp(){
	int menuSize = 250;
	char menu[250] = {"\nPress Corresponding number to get information on commands \n 1. Getting the Version \n 2. Shutting down MPX-OS \n 3. Getting the time \n 4. Getting the Date \n 5. Setting the Time \n 6. Setting the Date \n 7. Exit Help\n\n"};
	sys_req(WRITE,DEFAULT_DEVICE, menu, &menuSize);
	char cmdBuffer[2];
	memset(cmdBuffer, '\0', 100);
	int bufferSize = 2;
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
		char message[300] = "Displays the current time of day.\n";		//TODO - Mohammed finish time help messages
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
	char prompt[50] = "Are you sure? \n 1. Yes \n 2. No\n";
	int ansInt;
	char ans[1];
	sys_req(WRITE, DEFAULT_DEVICE, prompt, &promptInt);
	sys_req(READ, DEFAULT_DEVICE, ans , &ansInt);
	int i = atoi(ans);
	
	if(i==1){
	
		klogv("MPX is shuting down");
		return 1;
	}
	else if(i==2){
	
		serial_print("shutdwon process is canceled by the user");
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
	int menuSize = 2500;
	char menu[2500] = {"\nPress Corresponding number to execute command \n 1. Version \n 2. Help \n 3. Shutdown \n 4. Get time \n 5. Get Date \n 6. Set Time \n 7. Set Date\n\n"};

	serial_println(" _   _  ___  ___  _____    __    _  _____  __        __  ___");   
	serial_println("| | | || __|| __||  _  |  |  \\  | ||  _  ||  \\      /  || __|");
	serial_println("| | | || |_ | |_ | |_| |  |   \\ | || |_| ||   \\    /   || |_ ");
	serial_println("| | | ||__ ||  _||  __ \\  | |\\ \\| ||  _  || |\\ \\  / /| ||  _|");
	serial_println("| |_| | _| || |_ | |  \\ \\ | | \\ \\ || | | || | \\ \\/ / | || |_ ");
	serial_println("|_____||___||___||_|   \\_\\|_|  \\__||_| |_||_|  \\__/  |_||___|");
	while(!quit){

		sys_req(WRITE,DEFAULT_DEVICE, menu, &menuSize);

		//get a command
		memset(cmdBuffer, '\0', 100);
		bufferSize = 99;
		sys_req(READ,DEFAULT_DEVICE,cmdBuffer,&bufferSize);
		int failSize = 100;
		char failure[100] = "Not a valid command, please enter a valid number.\n";
		int i = atoi(cmdBuffer);
		switch(i){
			case 1: version(); 					break;
			case 2: ourHelp();					break;
			case 3: quit = shutDown();				break;
			case 4: getTime();					break;
			case 5: getDate();					break;
			case 6: setTime();					break;
			case 7: setDate();					break;
			default: sys_req(WRITE,DEFAULT_DEVICE, failure, &failSize);
		}
	}
	return -1;

}



