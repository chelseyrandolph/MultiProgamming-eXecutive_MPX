#include <string.h>
#include "hex.h"
#include "../../lib/colortext.h"

void printDecToHex(int dec){
	
	char hex[8];

	int i = 0;
	while(dec != 0){
	
		int temp = dec%16;

		if(temp < 10){
			hex[i] = temp + 48;
			i++;
		}else{
			hex[i] = temp + 55;
			i++;
		}
		dec = dec/16;
	} 
	char outHex[8];
	outHex[0] = '0';
	outHex[1] = 'x';
	int k = 2;
	int j;
	for(j = i-1; j >= 0; j--){
		outHex[k] = hex[j];
		k++;
	}
	int length = sizeof(outHex)/sizeof(outHex[0]);
	outHex[length] = '\0';
	write_text_bold_cyan(outHex);
}

void test(){
	int num = 15;
	printDecToHex(num);
}
