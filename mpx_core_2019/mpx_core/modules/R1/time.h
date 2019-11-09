#ifndef time_check_h
#define time_check_h

#define Invalid_hour "you enter invalid type of hour"
#define Invalid_mint "you enter invalid type of mintuts"
#define Invalid_second "you enter incalid type of second"

/*
setTime prompts the user for a time to set, then uses the system's registers to set the interal time on the OS.
Return Type: Void
Paramters: None
*/
void setTime();


/*
getTime uses registers within the system to get the time set on the OS.
Return Type: Void
Paramters: None
*/
void getTime();


/*
itoa takes in an integer and returns a string that is usually used to print to the terminal.
Return Type: char*
Paramters: int: the number you are converting
*/
char* itoa(int num);

/*
BCDToDEC converts a binary number to a decimal number.
Return Type: int
Paramters: int: a biinary number being converted
*/
int BCDToDEC(int num);

/*
DECToBCD converts a decimal number to a binary number.
Return Type: int
Paramters: int: a decimal number being converted
*/
int DECToBCD(int num); 

#endif
