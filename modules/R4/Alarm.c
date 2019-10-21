
#include <string.h>
#include <system.h>
#include <core/io.h>
#include <core/serial.h>
#include "pcb.h" 
#include "../mpx_supt.h"
#include "../R1/time.h"
#include "../../lib/colortext.h"
#include <Alarm.h>

char introPrompt[25] = "Please enter alarm time \n\n";
char enterHour[20] = "Hours:  ";
char enterMin[20]  = "Minutes:  ";
char enterSec[20]  = "Seconds:  ";
char enterMsg[35]  = "Enter mess you want displayed:  ";

int msgSize = 35;

char userHour[5];
char userMin[5];
char userSec[5];
char userMessage[50];

int timeInt = 5;
int userInt = 50;

int hours;
int minutes;
int seconds;


void alarm(){
	write_text_bold_red(introPrompt);
	//Enter Hour
	memset(userHour,'\0', 5);
	sys_req(WRITE, DEAFULT_DEVICE, enterHour, &msgSize);
	sys_req(READ, DEFAULT_DEVICE, userHour, &timeInt);
	hours = atoi(userHour);
	//Enter Minute
	memset(userMin,'\0', 5);
	sys_req(WRITE, DEAFULT_DEVICE, enterMin, &msgSize);
	sys_req(READ, DEFAULT_DEVICE, userMin, &timeInt);
	minutes = atoi(userMin);
	//Enter Second
	memset(userSec,'\0', 5);
	sys_req(WRITE, DEAFULT_DEVICE, enterSec, &msgSize);
	sys_req(READ, DEFAULT_DEVICE, userSec, &timeInt);
	seconds = atoi(userSec);
	//Enter Message
	memset(userMessage,'\0', 35);
	sys_req(WRITE, DEAFULT_DEVICE, enterMsg, &msgSize);
	sys_req(READ, DEFAULT_DEVICE, userMessage, &userInt);
	

	/*While loop that continuously checks if the alarm has hit the current time
	   If it hits the current time then we print the user message, then the process exits
	   If the current time does not match, then the process will print "not time yet" then the process voluntarily idles  */
	while(1){
		char notTime[10] = "\nnot time yet\n";
		if(checkTime(hours, minutes, seconds){
			sys_req(WRITE, DEFAULT_DEVICE, userMessage, &userInt);
			sys_req(EXIT, DEFAULT_DEVICE, null, null);
		}	
		sys_req(WRITE, DEFAULT_DEVICE, notTime, &msgSize);
		sys_req(IDLE, DEFAULT_DEVICE, null, null);
	}
	
	
}

/*
	This function checks the current time using the same functions from getTime in time.c
		then it checks the hours and minutes against the user hour and minutes
		if it passes, then it returns a 1 or yes otherwise returns a 0 or no
*/
int checkTime(int hours, int minutes, int seconds){
	int hourr, minutee , secondd;

	//Hours
	outb(0x70,HOUR);
	hourr= inb(0x71);
	hourr = hourr-((int) hourr/16)*6;

	//Minutes
	outb(0x70,MINUTE);
	minutee= inb(0x71);
	minutee = minutee-((int) minutee/16)*6;

	//Seconds
	outb(0x70,SECOND);
	secondd = inb(0x71);
	secondd = secondd-((int) secondd/16)*6;

	if(hours==hourr && minutes ==minutee){
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
