#include <string.h>
#include <core/io.h>
#include "mpx_supt.h"
#include <core/serial.h>
#include "setTime.h"
#define HOUR 0x04
#define Minute 0x02
#define Second 0x00


void SetTime() {


int hourToint, minToint, secToint;
int sethour=0 ,setmint =0,setsec ;
char hourInput[20] ,hour,mintInput[20],mint,secInput[20],sec;
 int entersize = 10 ,size =3;

//invalid interupt
cli();

serial_println("Enter the hour :", hourInput);
sys_req(WRITE, DEFAULT_DEVICE, hourInput, &entersize);
memset(hour,3);
sys_req(READ, DEFAULT_DEVICE, hour,&size);

hourToint= atoa(hour);
sethour = hourToint;
hourToint = DecToBCD(hourToint);



if(sethour < 0 || sethour>23){

int Ihoursize = sizeof(Invalid_hour);
sys_req(WRITE, DEFAULT_DEVICE,Invalid_hour,&Ihoursize);}
else{
outb(0x70,0x04);
outb(0x71,hourToint);}


serial_println("Enter the minutes :", mintInput);
sys_req(WRITE, DEFAULT_DEVICE, minttInput, &entersize);
//memset(mint,3);
sys_req(READ, DEFAULT_DEVICE, mint,&size);

minToint= atoa(mint);
setmint = mintToint;
mintToint = DecToBCD(mintToint);



if(setmint < 0 || setmint>59){

int Imintsize = sizeof(Invalid_min);
sys_req(WRITE, DEFAULT_DEVICE,Invalid_mint,&Imintsize);}
else{
outb(0x70,0x02);
outb(0x71,minToint);}


serial_println("Enter the second :", sectInput);
sys_req(WRITE, DEFAULT_DEVICE, secInput, &entersize);
memset(sec,3);
sys_req(READ, DEFAULT_DEVICE, sec,&size);

secToint= atoa(sec);
setsec = secToint;
secToint = DecToBCD(secToint);



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


outb(0x70,Hour);
hour= inb(0x71);
hour = hour-((int) hour/16)*6;


outb(0x70,Minute);
minute= inb(0x71);
minute = minute-((int) minute/16)*6;


outb(0x70,Second);
second= inb(0x71);
second = second-((int) second/16)*6;



serial_println("the current time is : ");
int h = itoa(hour);
serial_println(h);

int m = itoa(minute);
serial_println(m);

int s = itoa(second);

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

