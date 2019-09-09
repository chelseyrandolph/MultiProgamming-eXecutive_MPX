#include <system.h>
#include <core/io.h>
#include <core/serial.h>
#include <string.h>
#include "mpx_supt.h"
#include "comhand.h"

void version();
void ourHelp();
int shutDown();
void getTime();
void getDate();
void setTime();
void setDate();


int comhand(){
	char cmdBuffer[100];
	int bufferSize;
	int quit = 0;
	int menuSize = 2500;
	char menu[2500] = {"\nPress Corresponding number to execute command \n 1. Version \n 2. Help \n 3. Shutdown \n 4. Get time \n 5. Get Date \n 6. Set Time \n 7. Set Date\n\n"};
	sys_req(WRITE,DEFAULT_DEVICE, menu, &menuSize);

	while(!quit){
		//get a command
		memset(cmdBuffer, '\0', 100);
		bufferSize = 99;
		sys_req(READ,DEFAULT_DEVICE,cmdBuffer,&bufferSize);
		int failSize = 100;
		char failure[100] = "Not a valid command, please enter a valid number\n";
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

void version(){
	int verSize = 10;
	char version[10] = "1.1\n";
	sys_req(WRITE,DEFAULT_DEVICE, version, &verSize);
}

void ourHelp(){}


int shutDown(){
	int promptInt = 50;
	char prompt[50] = "Are you sure? \n 1. Yes \n 2. No\n";
	int ansInt;
	char ans[10];

	sys_req(WRITE, DEFAULT_DEVICE, prompt, &promptInt);
	sys_req(READ, DEFAULT_DEVICE, ans , &ansInt);
	int i = atoi(ans);
	if(i==1){
		return 1;
	}
	else if(i==2){
		return 0;
	}else{
		int failedAbortSize = 50;
		char failedAbort[50] = "\nNot valid command, aborting to main menu\n";
		sys_req(WRITE, DEFAULT_DEVICE, failedAbort, &failedAbortSize);
		return 0;
	}
}

void getTime(){}
void getDate(){}
void setTime(){}
void setDate(){}


//		" ____ ___                                                 "                    
//		"|    |   \______ ___________  ____ _____    _____   ____  "
//		"|    |   /  ___// __\\_  __\\/    \\__ \\  /    \\_/ __\\ "
//		"|    |  /\\___\\  ___/|  |\\/   | \\/ __\\|  Y Y \\  ___/ "
//		"|______//____ >\\___  >__|  |___|  (____  /__|_|  /\\___ >"
//		"            \\/    \\/          \\/    \\/     \\/    \\/ 
//		"		  /    \\   ____   ____  __ __ 		   "
//		"		 /  \ / \\_/ __\\ /   \\|  | \\		   "
//		"		/    Y   \\  ___/|   | \\  |  /		   "
//		"	       \\____|__ /\\___  >___|  /____/ 		   "
//		"		       \\/    \\/    \\/       		   "

