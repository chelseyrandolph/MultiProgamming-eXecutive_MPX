/* 
init_polling reads in from a port (COM1). This bytes read in is then compared to each case in a switch statement to figure out how to handle it. This basically reads user input. 
Return Type: int
Paramters: char*: the buffer that the user is inputting into
	 	   int: the counter for the end of the buffer
*/
int init_polling(char *buffer, int *count);


