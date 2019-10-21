#include <core/serial.h>
#include <core/interrupts.h>
#include <core/io.h>
#include <string.h>
#include "../mpx_supt.h"
#include "date.h"
#include "time.h"


void getDate(){

	//Variables 
	char bckslash[2] = "/";
	int bsSize = 2;
	int imonth;
	int iday;
	int iyear;
	int iyear2;
	char *cmonth;
	char *cday;
	char *cyear;
	char *cyear2;
	int monthSize;
	int daySize;
	int yearSize;
	int yearSize2;
	int msg_size = 30;

	sys_req(WRITE, DEFAULT_DEVICE, "The date is : \033[1;36m", &msg_size);

	//Getting the month

	outb(0x70, 0x08);
	imonth = inb(0x71);
	imonth = BCDToDEC(imonth);
	cmonth = itoa(imonth);
	monthSize = strlen(cmonth);
	sys_req(WRITE, DEFAULT_DEVICE, cmonth, &monthSize);
	sys_req(WRITE, DEFAULT_DEVICE, bckslash, &bsSize);

	//Getting the day
	outb(0x70, 0x07);
	iday = inb(0x71);
	iday = BCDToDEC(iday);
	cday = itoa(iday);
	daySize = strlen(cday);
	sys_req(WRITE, DEFAULT_DEVICE, cday, &daySize);
	sys_req(WRITE, DEFAULT_DEVICE, bckslash, &bsSize);

	//Getting the year
	outb(0x70, 0x32);
	iyear = inb(0x71);
	iyear = BCDToDEC(iyear);
	cyear = itoa(iyear);
	yearSize = strlen(cyear);
	sys_req(WRITE, DEFAULT_DEVICE, cyear, &yearSize);

	outb(0x70, 0x09);
	iyear2 = inb(0x71);
	iyear2 = BCDToDEC(iyear2);
	cyear2 = itoa(iyear2);
	yearSize2 = strlen(cyear2);
	sys_req(WRITE, DEFAULT_DEVICE, cyear2, &yearSize2);

	
	sys_req(WRITE, DEFAULT_DEVICE, "\033[0m\n\n", &bsSize);
}


void setDate(){

	//Variables
	int size = 3;	
	int ySize = 5;
	int tmp_month = 0;
	int tmp_day = 0;
	int tmp_year2 = 0;
	char month[3];
	char day[3];
	char year[5];
	int imonth;
	int iday;
	int iyear1;
	int iyear2;

	

	//Month
	char getMonth[50] = "Enter a month (MM): ";
	sys_req(WRITE, DEFAULT_DEVICE, getMonth, &size);
	memset(month, '\0',3);
	sys_req(READ, DEFAULT_DEVICE, month, &size);
	imonth = atoi(month);
	tmp_month = imonth;
	imonth = DECToBCD(imonth);

	//Day
	char getDay[50] = "Enter a day (DD): ";
	sys_req(WRITE, DEFAULT_DEVICE, getDay, &size);
	memset(day, '\0',3);
	sys_req(READ, DEFAULT_DEVICE, day, &size);
	iday = atoi(day);
	tmp_day = iday;
	iday = DECToBCD(iday);

	//Year
	char getYear[50] = "Enter a year (1700 - 2100) (YYYY): ";
	sys_req(WRITE, DEFAULT_DEVICE, getYear, &ySize);
	memset(year, '\0',5);
	sys_req(READ, DEFAULT_DEVICE, year, &ySize);
	
	char year1[3];
	char year2[3];
	
	

	year1[0] = year[0];
	year1[1] = year[1];
	year1[2] = '\0';
	
	year2[0] = year[2];
	year2[1] = year[3];
	year2[2] = '\0';
	iyear1 = atoi(year1);
	iyear1 = DECToBCD(iyear1);

	iyear2 = atoi(year2);
	tmp_year2 = iyear2;
	iyear2 = DECToBCD(iyear2);
	//Checking to make sure the dates are valid
	int valid = 0;
	int integer_year;
	integer_year = atoi(year);
	int daysAmonth[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

	while(!valid){
		if((tmp_month > 0 && tmp_month < 13) && (tmp_day > 0 && tmp_day <= daysAmonth[tmp_month-1]) && (integer_year > 1699 && integer_year < 2101)){
			valid = 1;
		//Handling leap years with February
		}else if(tmp_month == 2 && tmp_year2 % 4 == 0 && tmp_day == 29){
			valid = 1;
		}else{


			//THIS IS WHERE THE ERROR IS
			serial_println("Error: INVALID DATE --- Please enter a valid date in MM/DD/YYYY format\n");


			sys_req(WRITE, DEFAULT_DEVICE, getMonth, &size);
			memset(month, '\0',3);
			sys_req(READ, DEFAULT_DEVICE, month, &size);
			imonth = atoi(month);
			tmp_month = imonth;
			imonth = DECToBCD(imonth);
	
			sys_req(WRITE, DEFAULT_DEVICE, getDay, &size);
			memset(day, '\0',3);
			sys_req(READ, DEFAULT_DEVICE, day, &size);
			iday = atoi(day);
			tmp_day = iday;
			iday = DECToBCD(iday);
		
			sys_req(WRITE, DEFAULT_DEVICE, getYear, &ySize);
			memset(year, '\0',5);
			sys_req(READ, DEFAULT_DEVICE, year, &ySize);
			char year1[3];
			char year2[3];
	
	

			year1[0] = year[0];
			year1[1] = year[1];
			year1[2] = '\0';
	
			year2[0] = year[2];
			year2[1] = year[3];
			year2[2] = '\0';

			iyear1 = atoi(year1);
			iyear1 = DECToBCD(iyear1);

			iyear2 = atoi(year2);
			tmp_year2 = iyear2;
			iyear2 = DECToBCD(iyear2);

			break;
		}
	
	}

	//Actually setting the date
	//Disable interrupts
	cli();
	//month
	outb(0x70, 0x08);
	outb(0x71, imonth);
	//day 
	outb(0x70, 0x07);
	outb(0x71, iday);
	//year
	outb(0x70, 0x32);
	outb(0x71, iyear1);
	outb(0x70, 0x09);
	outb(0x71, iyear2);
	//Enable interrupts
	sti();
}	

