/*
  ----- polling.c -----
	
*/

#include <system.h>
#include <core/io.h>
#include <core/serial.h>
#include <string.h>
#include "mpx_supt.h"
#include "polling.h"


char tmp_buf[4];

int init_polling(char *buffer, int *count){
	char str[2];
	str[1] = '\0';

	if(!buffer || !count){
		return -1;
	}
	//Where your cursor is at in the buffer and how long is the buffer.
	int cursor = 0;
	int buf_len = 0;

	while(1){
		if( inb(COM1 + 5) & 1){
			char letter = inb(COM1);
			//Depending on what letter is given by the user, 
			//depends on how it's handled in this polling function.
			switch(letter){
				case 127: //backspace
					//Option 1: If the cursor isn't 0 and is equal to the length 
					//of the buffer. (It's at the end of the line.) Delete the character
					//and move the cursor over, subtract one from the length of the buffer.
					if(cursor != 0){
							//If the cursor is at the end of the buffer
							if(cursor == buf_len){
								buffer[cursor - 1] = 00;
								serial_print("\b \b");
								cursor--;
								buf_len--;

					//Option 2: If the cursor is in the buffer (not at the end), delete the 
					//highlighted character, move the cursor, and loops throughout the buffer 
					//adding the buffer of everything to the right to everything on 
					//the left side. Subtract one from the total length of the buffer.
							}else{
								serial_print("\b \b");
								int temp_len = buf_len;
								int temp_cursor = cursor;
								//What needs to be reprinted back out after the key is backspaced.
								int buf_2 = temp_len - temp_cursor;
								//Fill in the rest of the line with the remaining characters
								int i = 0;
								for(i = 0; i < buf_2; i++){
									buffer[cursor + i - 1] = buffer[cursor + i];
									letter = buffer[cursor + i];
									str[0] = letter;
									serial_print(str);
								}
								serial_print(" ");
								serial_print("\b \b");
								buffer[buf_len - 1] = 00;
								cursor--;
								buf_len--;

								for(i = 0; i < buf_2; i++){
									serial_print("\b");
								}
							}	
					}
					break;
				case 13: //return
					serial_print("\r\n");			

					break;
				case 89: 
					tmp_buf[4] = '\0';
					int tmp_len = 0;
					//Store the code in a buffer
					while (inb(COM1+5)&1){
						tmp_buf[tmp_len] = inb(COM1);
						tmp_len++;
					}
					//A bracket '[' was the first character used
					if(tmp_buf[0] == 91){
						switch(tmp_buf[1]){
							// \033[3~    ???????? maybe
							// Delete
							//Options are the same as the backspace key, except everything 
							//to the right of the highlighted character gets deleted.
							case 51:
								if(cursor < buf_len){
									str[0] = buffer[cursor];
									serial_print(str);
									serial_print("\b \b");
								
									// Creating temporary values to fix the buffer once
									// the character has been deleted.
									int t_len = buf_len;
									int t_cursor = cursor;
									int buf_3 = t_len - t_cursor;
									int i = 0;
									//Since we deleted the character to the right of the cursor,
									//we will shift all the characters into the space before them 
									//in the buffer then print out the new string.
									for(i = 0; i < buf_3; i++){
										buffer[cursor + i] = buffer[cursor + i + 1];
										letter = buffer[cursor + i ];
										serial_print(str);
									}
									serial_print(" ");
									serial_print("\b \b");
									buf_len--;
									
									for(i = 0; i < buf_3 - 1; i++){
										serial_print("\b");
									}
								}
							break;
						

							//Keep track of each arrow key to move the cursor over so if 
							//they decide to type characters, we can handle that as needed.
							case 67: //right arrow
								if(cursor < buf_len){
									if(buffer[cursor] == 127){
										str[0] = buffer[cursor];
										serial_print(str);
										cursor++;
									}else{
										str[0] = buffer[cursor];
										serial_print(str);
										cursor++;
										
									}
								}
								
							break;

							case 68: //left arrow
								if(cursor > 0){
									serial_print("\b");
									cursor--;
								}
							break;
						}
					}
				//Default is for any of letters/numbers a-zA-Z0-9
				default: //other letters and numbers
					
					// if cursor isn't at the end of the buff that we
					// are giving the user to put input into
					if(cursor + 1 != *count ) {
						if(cursor == buf_len) {
							buffer[cursor] = letter;
							cursor++;
							buf_len++;
							str[0] = letter;
							serial_print(str); //prints the letter to the terminal
						// If we are adding more to their input within the input they
						// already typed.
						} else {
							int temp_c = cursor;
							int i;
							// The position in front of the cursor, where
							// we will type next
							int temp_len = buf_len + 1;
							// While the cursor is less than the length of the string
							/*while(temp_c < temp_len){
								//The characters are shifted over from where the cursor was  									//to the next space.
								buffer[temp_len] = buffer[temp_len - 1];
								temp_len--;
							}*/
							buffer[cursor] = letter;
							cursor++;
							buf_len++;
							str[0] = letter;
							serial_print(str);
							temp_c = cursor;
							temp_len = buf_len + 1;
							for(i = 0; i < (temp_c - temp_len); temp_len++){
								letter = buffer[temp_len];
								str[0] = letter;
								serial_print(str);
							}

							i = 0;
							temp_c = cursor;
							temp_len = buf_len + 1;
							for(i = 0; i < (temp_c - temp_len) - 1; temp_len++){
								serial_print("\b");
							}				
						}
					}
			}
		}
	}
	return 1;
}


