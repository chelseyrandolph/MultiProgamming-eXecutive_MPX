#include <system.h>
#include <core/io.h>
#include <core/serial.h>
#include <string.h>
#include "../mpx_supt.h"
#include "comhand.h"
#include "time.h"
#include "date.h"

char commands[9][30] = {
    "version",
    "help",
    "shut-down",
    "get-time",
    "get-date",
    "set-time",    
    "set-date",
	"create-pcb",
	"show-all",
};
