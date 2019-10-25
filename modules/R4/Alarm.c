
#include <string.h>
#include <system.h>
#include <core/io.h>
#include <core/serial.h>
#include "../mpx_supt.h"
#include "../R1/time.h"
#include "../../lib/colortext.h"
#include "Alarm.h"
#include "Dispatcher.h"

#define HOUR 0x04
#define MINUTE 0x02


static int timeInt = 5;
static int userInt = 50;


alarmQueue alarm_queue = {.head = NULL, .tail = NULL, .count = 0};


void remove_alarm(alarm* ralarm){
	if(alarm_queue.count ==1){
		alarm_queue.head = NULL;
		alarm_queue.tail = NULL;
		alarm_queue.count--;
	}else{
		alarm *temp = alarm_queue.head;
		if(temp == ralarm){
			alarm_queue.head = temp->next;
			alarm_queue.count--;
		}
		while(temp != ralarm){
			temp = temp->next;
			if(temp->next == ralarm){
				if(temp->next->next == NULL){
					temp->next = ralarm->next;
				}else{
					alarm_queue.tail = temp;
				}
				alarm_queue.count--;
			}	
		}
	}
	sys_free_mem(ralarm);	
}
void setAlarm(){
	if(find_pcb("alarm_check") == NULL){
		if(alarm_queue.count != 0){
			
		}
		loadProcess("sys_load_proc", 0, 0, &sys_load_proc);
		loadProcess("alarm_check", 1, 2, &checkAlarms);
	}
	alarm *new_alarm = sys_alloc_mem(sizeof(alarm));	//allocate mem to the alarm
	
	write_text_bold_blue("Please enter alarm time \n");

	//Enter Hour
	char userHour[5];
	memset(userHour,'\0', 5);
	write_text("Hours:  ");
	sys_req(READ, DEFAULT_DEVICE, userHour, &timeInt);
	new_alarm->Hour = atoi(userHour);

	//Enter Minute
	char userMin[5];
	memset(userMin,'\0', 5);
	write_text("Minutes:  ");
	sys_req(READ, DEFAULT_DEVICE, userMin, &timeInt);
	new_alarm->Minute = atoi(userMin);

	//Enter Message
	char userMessage[50];
	write_text("Enter message you want displayed:  ");
	sys_req(READ, DEFAULT_DEVICE, userMessage, &userInt);
	strcpy(new_alarm->message, userMessage);
//  	INSERTS NEW ALARM

	if(alarm_queue.count == 0){		//If there arent any alarms, first alarm becomes head and tail
		alarm_queue.head = new_alarm;
		alarm_queue.tail = new_alarm;
		alarm_queue.count++;
	}else{							//Append any additional alarms to tail of queue
		alarm *old_tail = alarm_queue.tail;
		old_tail->next = new_alarm;
		alarm_queue.tail = new_alarm;
		alarm_queue.count++;
	}

//==========================================================================


	/*While loop that continuously checks if the alarm has hit the current time
	   If it hits the current time then we print the user message, then the process exits
	   If the current time does not match, then the process will print "not time yet" then the process voluntarily idles  */
	/*while(1){
		
		if(checkTime(hours, minutes)){
			sys_req(WRITE, DEFAULT_DEVICE, userMessage, &userInt);
			sys_req(EXIT, DEFAULT_DEVICE, NULL, NULL);
		}	
		write_text("Not time yet\n");
		sys_req(IDLE, DEFAULT_DEVICE, NULL, NULL);
	}*/
	
	
}

/*
	This function checks the current time using the same functions from getTime in time.c
		then it checks the hours and minutes against the user hour and minutes
		if it passes, then it returns a 1 or yes otherwise returns a 0 or no
*/
void checkAlarms(){
	if(alarm_queue.count == 0){
		sys_req(EXIT, DEFAULT_DEVICE, NULL, NULL);	//ends the alarm process if there are no alarms
	}else{
		alarm *temp_alarm = alarm_queue.head;
		while(1){
			int int_hour, int_minute;
			//Hours
			outb(0x70, HOUR);
			int_hour= inb(0x71);
			int_hour = int_hour-((int) int_hour/16)*6;

			//Minutes
			outb(0x70,MINUTE);
			int_minute= inb(0x71);
			int_minute = int_minute-((int) int_minute/16)*6;

	
			if(temp_alarm->Hour == int_hour && temp_alarm->Minute == int_minute){
				//sys_req(WRITE, DEFAULT_DEVICE, temp_alarm->message, &userInt);
				write_text_bold_green("ALARM COMPLETE: ");
				write_text_green(temp_alarm->message);
				remove_alarm(temp_alarm);
				//klogv("remove");
				if(alarm_queue.count == 0){
					sys_req(EXIT, DEFAULT_DEVICE, NULL, NULL);	//ends the alarm process if there are no alarms
				}
			}else{
				write_text_magenta("ALARM: Not time yet --- TIME: ");
				write_text_magenta("	ALARM SET TO: ");
				write_text_blue(itoa(int_hour));
				write_text_blue(itoa(int_minute));
				write_text_magenta("	ALARM SET TO: ");
				write_text_green(itoa(temp_alarm->Hour));
				write_text_green(itoa(temp_alarm->Minute));
				write_text_magenta("	Message: ");
				write_text_cyan(temp_alarm->message);
				write_text("\n");
			
			}
			if(temp_alarm != alarm_queue.tail){
				temp_alarm = temp_alarm->next;	
			}else{
				sys_req(IDLE, DEFAULT_DEVICE, NULL, NULL);
				temp_alarm = alarm_queue.head;
			}
			
		}
		//sys_req(EXIT, DEFAULT_DEVICE, NULL, NULL);
		
	}

	
}


/*	pseudocode

	while(not at the alarm time){
		sys_req(idle)
	}
	sys_req(write user message)
	sys_req(EXIT)
	

*/
