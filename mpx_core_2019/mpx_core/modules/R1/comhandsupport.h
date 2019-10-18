#include <system.h>
#include <core/io.h>
#include <core/serial.h>
#include <string.h>
#include "../mpx_supt.h"
#include "comhand.h"
#include "time.h"
#include "date.h"

char commands[20][30] = {
    "version",
    "help",
    "shutdown",
    "get-time",
    "get-date",
    "set-time",    
    "set-date",
	"create-pcb",
	"show-all",
	"show-blocked",
	"show-ready",
    "show-pcb",
	"delete-pcb",
	"suspend-pcb",
	"resume-pcb",
	"block-pcb",
	"unblock-pcb",
	"set-pcb-priority",
	"loadr3"
};
//Paramarized help menu 
char helpcommands[20][70] = {
    "version",
    "help [command]",
    "shutdown",
    "get-time",
    "get-date",
    "set-time",    
    "set-date",
	"create-pcb \033[0;34m[name] [class] [priority]\033[0m",
	"show-all",
	"show-blocked",
	"show-ready",
    "show-pcb \033[0;34m[pcb_name]\033[0m",
	"delete-pcb \033[0;34m[pcb_name]\033[0m",
	"suspend-pcb \033[0;34m[pcb_name]\033[0m",
	"resume-pcb \033[0;34m[pcb_name]\033[0m",
	"block-pcb \033[0;34m[pcb_name]\033[0m",
	"unblock-pcb \033[0;34m[pcb_name]\033[0m",
	"set-pcb-priority \033[0;34m[pcb_name] [new_priority]\033[0m",
	"loadr3"
};
