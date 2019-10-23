
#include <string.h>
#include <system.h>
#include <core/io.h>
#include <core/serial.h>
#include "pcb.h" 
#include "../mpx_supt.h"
#include "../R1/time.h"
#include "../../lib/colortext.h"
#include "Alarm.h"

#define HOUR 0x04
#define MINUTE 0x02

static char userHour[5];
static char userMin[5];
static char userMessage[50];

static int timeInt = 5;
static int userInt = 50;

int hours;
int minutes;

/* How does this implementation handle multiple alarms? 
We should also make a funcion to display all the current alarms that the user had made.*/


void alarm(){
	write_text_bold_red("Please enter alarm time \n\n");

	//Enter Hour
	memset(userHour,'\0', 5);
	write_text("Hours:  ");
	sys_req(READ, DEFAULT_DEVICE, userHour, &timeInt);
	hours = atoi(userHour);

	//Enter Minute
	memset(userMin,'\0', 5);
	write_text("Minutes:  ");
	sys_req(READ, DEFAULT_DEVICE, userMin, &timeInt);
	minutes = atoi(userMin);

	//Enter Message
	memset(userMessage,'\0', 35);
	write_text("Enter message you want displayed:  ");
	sys_req(READ, DEFAULT_DEVICE, userMessage, &userInt);
	

	/*While loop that continuously checks if the alarm has hit the current time
	   If it hits the current time then we print the user message, then the process exits
	   If the current time does not match, then the process will print "not time yet" then the process voluntarily idles  */
	while(1){
		
		if(checkTime(hours, minutes)){
			sys_req(WRITE, DEFAULT_DEVICE, userMessage, &userInt);
			sys_req(EXIT, DEFAULT_DEVICE, NULL, NULL);
		}	
		write_text("Not time yet\n");
		sys_req(IDLE, DEFAULT_DEVICE, NULL, NULL);
	}
	
	
}

/*
	This function checks the current time using the same functions from getTime in time.c
		then it checks the hours and minutes against the user hour and minutes
		if it passes, then it returns a 1 or yes otherwise returns a 0 or no
*/
int checkTime(int current_hour, int current_minute){
	int int_hour, int_minute;
	//Hours
	outb(0x70, HOUR);
	//hourr= inb(0x71);
	int_hour = HOUR-((int) HOUR/16)*6;

	//Minutes
	outb(0x70,MINUTE);
	//minutee= inb(0x71);
	int_minute = MINUTE-((int) MINUTE/16)*6;


	if(current_hour == int_hour && current_minute == int_minute){
		return 1;
	}else{
		return 0;
	}
}


/*	pseudocode

	while(not at the alarm time){
		sys_req(idle)
	}
	sys_req(write user message)
	sys_req(EXIT)
	

*/
