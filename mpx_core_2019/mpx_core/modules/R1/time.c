#include <string.h>
#include <core/io.h>
#include <core/serial.h>
#include <core/interrupts.h>
#include <system.h>
#include "../mpx_supt.h"
#include "time.h"
#include <stdbool.h>

#define Invalid_hour "you enter invalid type of hour"
#define Invalid_min "you enter invalid type of mintuts"
#define Invalid_sec "you enter incalid type of second"

#define HOUR 0x04
#define MINUTE 0x02
#define SECOND 0x00


void setTime() {


int hourToint, minToint, secToint;
int sethour=0 ,setmin =0,setsec =0;
char hourInput[20] ,minInput[20],secInput[20];
int size =3;

//invalid interupt
cli();

serial_print("Enter the hour : ");
//sys_req(WRITE, DEFAULT_DEVICE, hourInput, &entersize);
memset(hourInput, '\0', 3);
sys_req(READ, DEFAULT_DEVICE, hourInput, &size);

hourToint= atoi(hourInput);
sethour = hourToint;
hourToint = DECToBCD(hourToint);



if(sethour < 0 || sethour>23){

int Ihoursize = sizeof(Invalid_hour);
sys_req(WRITE, DEFAULT_DEVICE,Invalid_hour,&Ihoursize);
}
else{
outb(0x70,0x04);
outb(0x71,hourToint);}


serial_print("Enter the minutes  : ");
//sys_req(WRITE, DEFAULT_DEVICE, minInput, &entersize);
memset(minInput, '\0', 3);
sys_req(READ, DEFAULT_DEVICE, minInput,&size);

minToint= atoi(minInput);
setmin = minToint;
minToint = DECToBCD(minToint);



if(setmin < 0 || setmin>59){

int Imintsize = sizeof(Invalid_min);
sys_req(WRITE, DEFAULT_DEVICE,Invalid_min,&Imintsize);}
else{
outb(0x70,0x02);
outb(0x71,minToint);}


serial_print("Enter the second : ");
//sys_req(WRITE, DEFAULT_DEVICE, secInput, &entersize);
memset(secInput, '\0', 3);
sys_req(READ, DEFAULT_DEVICE, secInput ,&size);

secToint= atoi(secInput);
setsec = secToint;
secToint = DECToBCD(secToint);



if(setsec < 0 || setsec>59){


int Isecsize = sizeof(Invalid_sec);
sys_req(WRITE, DEFAULT_DEVICE,Invalid_sec,&Isecsize);}
else{
outb(0x70,0x00);
outb(0x71,secToint);}

// return inturupt
sti();
}



void getTime(){
	int hourr, minutee , secondd;


	outb(0x70,HOUR);
	hourr= inb(0x71);
	hourr = hourr-((int) hourr/16)*6;


	outb(0x70,MINUTE);
	minutee= inb(0x71);
	minutee = minutee-((int) minutee/16)*6;


	outb(0x70,SECOND);
	secondd = inb(0x71);
	secondd = secondd-((int) secondd/16)*6;


	int msg_size = 30;
	sys_req(WRITE, DEFAULT_DEVICE, "the current time is : \033[1;36m  ", &msg_size);
	int digit_size = 3;
	char *h;
	h = itoa(hourr);
	//h = itoi(hourr);
	sys_req(WRITE, DEFAULT_DEVICE, h, &digit_size);
	sys_req(WRITE, DEFAULT_DEVICE, ":", &digit_size);
	char *m;
	m = itoa(minutee);
	//m = itoi(minutee);
	sys_req(WRITE, DEFAULT_DEVICE, m, &digit_size);
	sys_req(WRITE, DEFAULT_DEVICE, ":", &digit_size);
	char *s;
	s = itoa(secondd);
	//s = itoi(secondd);
	sys_req(WRITE, DEFAULT_DEVICE, s, &digit_size);
	
	sys_req(WRITE, DEFAULT_DEVICE, "\033[0m\n\n", &msg_size);

}

char* itoa(int num)
{
	char* str = " ";
	int base = 10;
	bool isNegative = false;

	/* Handle 0 explicitely, otherwise empty string is printed for 0 */
	if (num == 0)

	{
		str[0] = '0';
		str[1] = '\0';
		return str;
	}

	// In standard itoa(), negative numbers are handled only with
	// base 10. Otherwise numbers are considered unsigned.
	if (num < 0 && base == 10)
	{
		isNegative = true;
		num = -num;
	}
	// len will store the length of integer
	//e.g len of 188 is 3 if base is 10
	int len = 0;
	if (isNegative)
		len++;
	int temp = num;
	while (temp != 0)
	{
		len++;
		temp /= base;
	}
	//null char is placed at end of str
	str[len--] = '\0';
	// Process individual digits
	while (num != 0)
	{
		int rem = num % base;
		char ch;
		if (rem > 9)
		{
			ch = (rem - 10) + 'A';
		}
		else
		{
			ch = rem + '0';
		}
		//the rem is placed to it's position 
		str[len--] = ch;
		num = num / base;
	}

	// If number is negative, append '-' at start
	if (isNegative)
		str[0] = '-';





	return str;
}

	// convert binary code decimal to string 
/*char* itoa(int number){

	char *string = " "; 
	//int i;
	//for(i=0 ;i <3; i++){

	string[0]= (number/10)+48;
	string[1]= (number%10)+48;
	string[2] = '\0';

	return string;




}*/


int BCDToDEC(int num){
return ((((num)>>4)&0x0F)*10) +(num&0x0F);

}

int DECToBCD(int num){
return ((num/10)*16)+(num%10);
}

