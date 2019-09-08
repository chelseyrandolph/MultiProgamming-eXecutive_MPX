#include <system.h>
#include <core/io.h>
#include <core/serial.h>
#include <string.h>
#include "mpx_supt.h"
#include "comhand.h"


int comHandler(const char * buffer){
	char cmdBuffer[100];
	int bufferSize;
	int quit = 0;
	int menuSize = 2000;
	char menu[menuSize] = {
		"__________________________________________________________"
		" ____ ___                                                 "
		"|    |   \______ ___________  ____ _____    _____   ____  "
		"|    |   /  ___// __ \_  __ \/     \__  \  /     \_/ __ \ "
		"|    |  /\___  \  ___/|  | \/   |  \/ __ \|  Y Y  \  ___/ "
		"|______//____  >\___  >__|  |___|  (____  /__|_|  /\___  >"
		"             \/     \/           \/     \/      \/     \/ "
		"		   _____                       
		"		  /     \   ____   ____  __ __ 		   "
		"		 /  \ /  \_/ __ \ /    \|  |  \		   "
		"		/    Y    \  ___/|   |  \  |  /		   "
		"		\____|__  /\___  >___|  /____/ 		   "
		"			\/     \/     \/       		   "
		"\n	Press Corresponding number to execute command	   "
		"	1. Version					   "
		"	2. Help						   "
		"	3. Shutdown					   "
		"	4. Get time					   "
		"	5. Get Date					   "
		"	6. Set Time					   "
		"	7. Set Date					   "
		"__________________________________________________________"
	};
	sys_req(WRITE,DEFAULT_DEVICE, menu, &menuSize);

	while(!quit){
		//get a command
		memset(buffer, "\0", 100);
		bufferSize = 99;
		sys_req(READ,DEFAULT_DEVICE,cmdBuffer,&bufferSize);
		int failSize = 100;
		char failure[failSize] = "Not a valid command, please enter a valid number";
		
		switch(cmBuffer){
			case 1: version(); 					break;
			case 2: help();						break;
			case 3: shutDown();					break;
			case 4: getTime();					break;
			case 5: getDate();					break;
			case 6: setTime();					break;
			case 7:	setDate();					break;
			default: sys_req(WRITE,DEFAULT, failure, &failSize);
		}
	}
}

void version(){
	double version = 1.0;
	sys_req(WRITE,DEFAULT_DEVICE, version, &version);
}


int shutDown(){
	int promptInt = 50;
	char prompt[promptInt] = "Are you sure? \n 1. Yes \n 2. No"
	int ansInt;
	char ans[10];

	sys_req(WRITE, DEFAULT_DEVICE, prompt, &promptInt);
	sys_req(READ, DEFAULT_DEVICE, ans , ansInt
	if(ans = "1"){
		return 1;
	}
	else{
		return 0;
	}
}


