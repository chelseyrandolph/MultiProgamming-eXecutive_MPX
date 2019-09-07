#include <core/serial.h>
#include <core/interrupts.h>
#include <core/io.h>
#include <string.h>
#include "mpx_supt.h"
#include "date.h"
#include "time.h"


void getDate(){

	//Variables 
	//char year[3] = "20";
	//int size = 3;
	char bckslash[2] = "/";
	int bsSize = 2;
	int imonth;
	int iday;
	int iyear;
	char *cmonth;
	char *cday;
	char *cyear;
	int monthSize;
	int daySize;
	int yearSize;

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
	outb(0x70, 0x09);
	iyear = inb(0x71);
	iyear = BCDToDEC(iyear);
	cyear = itoa(iyear);
	yearSize = strlen(cyear);
	sys_req(WRITE, DEFAULT_DEVICE, cyear, &yearSize);



}



void setDate(){

	//Variables
	//int qSize = 50;
	int size = 3;	
	int ySize = 3;
	int tmp_month = 0;
	int tmp_day = 0;
	int tmp_year = 0;
	char month[3];
	char day[3];
	char year[3];
	int imonth;
	int iday;
	int iyear;

	

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
	char getYear[50] = "Enter a year (YY): ";
	sys_req(WRITE, DEFAULT_DEVICE, getYear, &ySize);
	memset(year, '\0',3);
	sys_req(READ, DEFAULT_DEVICE, year, &ySize);
	iyear = atoi(year);
	tmp_year = iyear;
	iyear = DECToBCD(iyear);
	
	//Checking to make sure the dates are valid
	int valid = 0;
	int daysAmonth[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
	while(!valid){
	//If the month is within 1 and 12
	if(tmp_month > 0 && tmp_month < 13){
		if(tmp_day > 0 && tmp_day <= daysAmonth[tmp_month-1]){
			valid = 1;
		
		//Handling leap years with February
		}else if(tmp_month == 2 && tmp_year % 4 == 0 && tmp_day == 29){
			valid = 1;
		}else{
			serial_println("Error: INVALID DATE --- Please enter a valid date in MM/DD/YY format");
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
			memset(year, '\0',3);
			sys_req(READ, DEFAULT_DEVICE, year, &ySize);
			iyear = atoi(year);
			tmp_year = iyear;
			iyear = DECToBCD(iyear);

		}

		
		
		

	}else{
		serial_println("Error: INVALID DATE --- Please enter a valid date in MM/DD/YY format");
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
		iyear = atoi(year);
		tmp_year = iyear;
		iyear = DECToBCD(iyear);
	}
	}

	//Actually setting the date
	//Disable interrupts
	cli();
	//month
	sys_req(WRITE, DEFAULT_DEVICE, itoa(BCDToDEC(imonth)), &size);
	outb(0x70, 0x08);
	outb(0x71, imonth);
	//day 
	outb(0x70, 0x07);
	outb(0x71, iday);
	//year
	outb(0x70, 0x09);
	outb(0x71, iyear);
	//Enable interrupts
	sti();
}	

