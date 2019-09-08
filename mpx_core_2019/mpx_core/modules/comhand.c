#include <system.h>
#include <core/io.h>
#include <core/serial.h>
#include <string.h>
#include "mpx_supt.h"
#include "comhand.h"

int comhand(){
	char cmdBuffer[100];
	cmdBuffer[100] = '\0';
	int menuSize = 1000;
	int bufferSize;
	int quit = 0;

	while(!quit){
	char menu[1000] = "0: Quit\n1:\n2:\n";
	sys_req(WRITE, DEFAULT_DEVICE, menu, &menuSize);
	memset(cmdBuffer, '\0', 100);
	//reset before each all to read
	bufferSize = 99;
	sys_req(READ, DEFAULT_DEVICE, cmdBuffer, &bufferSize);
	
	if(*cmdBuffer == '0'){
		quit = 1;
	}else if(*cmdBuffer == '1'){
		serial_print("1\n");
	}else if(*cmdBuffer == '2'){
		serial_print("2\n");
	}else{
		serial_print("Please choose a command.\n");
	}

	}

	return -1;





}
