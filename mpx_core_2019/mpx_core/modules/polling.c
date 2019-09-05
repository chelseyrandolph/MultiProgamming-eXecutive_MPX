/*
  ----- polling.c -----
	
*/


#include "polling.h"



int init_polling(char *buffer, int *count){
	char str[2]'
	str[1] = '\0';

	if(!buffer || !count){
		return -1;
	}

//Where your cursor is at in the buffer and how long is the buffer.
	int cursor = 0;
	int buf_len = 0;

	while(1){
		if inb(COM1 + 5) & 1{
			char letter = inb(COM1);
			
			//Depending on what letter is given by the user, 
			//depends on how it's handled in this polling function.
			switch(letter){
				
				// 07xF ????   ^?
				case 12: //backspace
					
					//Option 1: If the cursor isn't 0 and is equal to the length 
					//of the buffer. (It's at the end of the line.) Delete the character
					//and move the cursor over, subtract one from the length of the buffer.
					if(cursor != 0){
							//If the cursor is at the end of the buffer
							if(cursor == buf_len){

								buffer[cursor - 1] = 00;
								//serial_print("\b \b");
								cursor--;
								buf_len--;

					//Option 2: If the cursor is in the buffer (Not at the end), delete the 
					//highlighted character, move the cursor, and loops throughout the buffer 
					//adding the buffer of everything to the right to everything on the left side. 
					//Subtract one from the total length of the buffer.
							}else{
								int temp_len = buf_len;
								int temp_cursor = cursor;
								//What needs to be reprinted back out after the key is backspaced.
								int buf_2 = temp_len - temp_cursor;
								//Fill in the rest of the line with the remaining characters
								for(int i = 0; i < buf_2; i++){
									buffer[cursor + i - 1] = buffer[cursor + i];
									letter = buffer[cursor + i];
									str[0] = letter;
								}
								
								buffer[buf_len - 1] = 00;
								cursor--;
								buf_len--;

							}
						
					}
					



					break;

				// \033[3~    ???????? maybe
				case 23: //Delete

					//Options are the same as the backspace key, except everything to the right 
					//of the highlighted character gets deleted.
					
					break;

				case 34: //return
					serial_print("\r\n");
					return 1;

					break;



				//Keep track of each arrow key to move the cursor over so if they decide to 
				//type characters, we can handle that as needed.
				case 45: //left arrow
					
					
					
					break;

				case 56: //right arrow
					
					break;
				//Default is for any of letters/numbers a-zA-Z0-9
				default: //other letters and numbers
					

			}
		}
		
	}
}


