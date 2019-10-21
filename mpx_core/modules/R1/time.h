#ifndef time_check_h
#define time_check_h

#define Invalid_hour "you enter invalid type of hour"
#define Invalid_mint "you enter invalid type of mintuts"
#define Invalid_second "you enter incalid type of second"

void setTime();
void getTime();
char* itoa(int number);
int BCDToDEC(int num);
int DECToBCD(int num); 

#endif
