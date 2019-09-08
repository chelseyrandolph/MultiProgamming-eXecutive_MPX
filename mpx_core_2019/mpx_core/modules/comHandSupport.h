#include<stdio.h>
#include<strind.h>

#define NUM_OF_COMMANDS = 7
#define MAX_LEN = 25

	
extern const char commands[NUM_OF_COMMANDS][MAX_LEN] {
	"Version",
	"Help",
	"Shutdown",
	"get time",
	"get date",
	"set time",
	"set date"
};

