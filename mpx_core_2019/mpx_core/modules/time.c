#include <string.h>
#include <core/io.h>
#include <core/serial.h>
#include <core/interrupts.h>

#include "mpx_supt.h"
#include "time.h"

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



	serial_print("the current time is :  ");

	char *h;
	h = itoa(hourr);
	serial_print(h);
	serial_print(":");
	char *m;
	m = itoa(minutee);
	serial_print(m);
	serial_print(":");
	char *s;
	s = itoa(secondd);
	serial_print(s);
	serial_print("\n");

}



	// convert binary code decimal to string 
	char* itoa(int number){

	char *string = " "; 
	//int i;
	//for(i=0 ;i <3; i++){

	string[0]= (number/10)+48;
	string[1]= (number%10)+48;
	string[2] = '\0';

	return string;


}

int BCDToDEC(int num){
return ((((num)>>4)&0x0F)*10) +(num&0x0F);

}

int DECToBCD(int num){
return ((num/10)*16)+(num%10);
}

