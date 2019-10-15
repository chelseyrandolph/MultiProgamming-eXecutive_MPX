#include <system.h>
#include <core/io.h>
#include <core/serial.h>
#include <string.h>
#include "../mpx_supt.h"
#include "comhand.h"
#include "time.h"
#include "date.h"

char commands[19][30] = {
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
	"set-pcb-priority"
};

char helpcommands[19][45] = {
    "version",
    "help 'command'",
    "shutdown",
    "get-time",
    "get-date",
    "set-time",    
    "set-date",
	"create-pcb",
	"show-all",
	"show-blocked",
	"show-ready",
    "show-pcb 'pcb_name'",
	"delete-pcb 'pcb_name'",
	"suspend-pcb 'pcb_name'",
	"resume-pcb 'pcb_name'",
	"block-pcb 'pcb_name'",
	"unblock-pcb 'pcb_name'",
	"set-pcb-priority 'pcb_name' 'new_priority'"
};
