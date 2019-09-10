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
#define Minute 0x02
#define Second 0x00


void setTime() {


int hourToint, minToint, secToint;
int sethour=0 ,setmin =0,setsec =0;
char hourInput[20] ,minInput[20],secInput[20];
int entersize = 10 ,size =3;

//invalid interupt
cli();

serial_println("Enter the hour :");
sys_req(WRITE, DEFAULT_DEVICE, hourInput, &entersize);
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


serial_println("Enter the minutes :");
sys_req(WRITE, DEFAULT_DEVICE, minInput, &entersize);
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


serial_println("Enter the second :");
sys_req(WRITE, DEFAULT_DEVICE, secInput, &entersize);
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
int hour, minute , second;


outb(0x70,hour);
hour= inb(0x71);
hour = hour-((int) hour/16)*6;


outb(0x70,minute);
minute= inb(0x71);
minute = minute-((int) minute/16)*6;


outb(0x70,second);
second= inb(0x71);
second = second-((int) second/16)*6;



serial_println("the current time is : ");

char *h;
h = itoa(hour);
serial_println(h);

char *m;
m = itoa(minute);
serial_println(m);

char *s;
s = itoa(second);
serial_println(s);


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
//}

}
int BCDToDEC(int num){
return ((((num)>>4)&0x0F)*10) +(num&0x0F);

}

int DECToBCD(int num){
return ((num/10)*16)+(num%10);
}

