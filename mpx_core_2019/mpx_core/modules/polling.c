/*
  ----- polling.c -----
	
*/


#include "polling.h"



int init_polling(char *buffer, int *count){
	while(1){
		if inb(COM1 + 5) & 1{
			char letter = inb(COM1);
			
			switch(letter){
				
				case 12: //backspace
					
					break;

				case 23: //Delete
					
					break;

				case 34: //return
					
					break;

				case 45: //left arrow
					
					break;

				case 56: //right arrow
					
					break;

				default: //other letters
					

			}
		}
		
	}
}

