#include <system.h>
#include <core/io.h>
#include <core/serial.h>
#include <string.h>
#include "../mpx_supt.h"
   
void displayMenu(){
	int menuSize = 2500;
	char menu[2500] = {"\nWelcome to MPX \nEnter the corresponding command to execute. \n version \n help \n shutdown \n getTime \n getDate \n setTime \n setDate\n\n"};

	serial_println(" _   _  ___  ___  _____    __    _  _____  __        __  ___");   
	serial_println("| | | || __|| __||  _  |  |  \\  | ||  _  ||  \\      /  || __|");
	serial_println("| | | || |_ | |_ | |_| |  |   \\ | || |_| ||   \\    /   || |_ ");
	serial_println("| | | ||__ ||  _||  __ \\  | |\\ \\| ||  _  || |\\ \\  / /| ||  _|");
	serial_println("| |_| | _| || |_ | |  \\ \\ | | \\ \\ || | | || | \\ \\/ / | || |_ ");
	serial_println("|_____||___||___||_|   \\_\\|_|  \\__||_| |_||_|  \\__/  |_||___|");

	sys_req(WRITE,DEFAULT_DEVICE, menu, &menuSize);
}
