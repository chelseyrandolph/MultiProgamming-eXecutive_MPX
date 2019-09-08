#include <system.h>
#include <core/io.h>
#include <core/serial.h>
#include <string.h>
#include "mpx_supt.h"
#include "comhand.h"

#include <system.h>
#include <core/io.h>
#include <core/serial.h>
#include <string.h>
#include "mpx_supt.h"
#include "comhand.h"


void version(){
	char version[20] = "Version 1.0";
	int ver_size = 20;
	sys_req(WRITE, DEFAULT_DEVICE, version, &ver_size);
}

int shutDown(){
	int promptInt = 50;
	char prompt[50] = "Are you sure? \n 1. Yes \n 2. No";
	int ansInt = 10;
	char ans[10];

	sys_req(WRITE, DEFAULT_DEVICE, prompt, &promptInt);
	sys_req(READ, DEFAULT_DEVICE, ans , &ansInt);
	if(*ans == '1'){
		return 1;
	}
	else{
		return 0;
	}
}



int comhand(){
	char cmdBuffer[100];
	cmdBuffer[100] = '\0';
	int menuSize = 1000;
	int bufferSize;
	int quit = 0;

	while(!quit){
		char menu[1000] = "\n\t1) Version\n\t2) Help\n\t3) Get Time\n\t4) Set Time\n\t5) Get Date\n\t6) Set Date\n\t7) Shutd0wn\n\n";
		sys_req(WRITE, DEFAULT_DEVICE, menu, &menuSize);
		memset(cmdBuffer, '\0', 100); 	//reset before each all to read
		bufferSize = 99;
		sys_req(READ, DEFAULT_DEVICE, cmdBuffer, &bufferSize);	
		int failSize = 100;
		char failure[100] = "Not a valid command, please enter a valid number";
		switch(atoi(cmdBuffer)){	// atoi = str -> int    
			case 1: version(); 					break;
			//case 2: help();						break;
			//case 3: getTime();					break;
			//case 4: setTime();					break;
			//case 5: getDate();					break;
			//case 6: setDate();					break;
			case 7:	shutDown();					break;
			default: sys_req(WRITE,DEFAULT_DEVICE, failure, &failSize);
		}
	}
	return -1;

}


















