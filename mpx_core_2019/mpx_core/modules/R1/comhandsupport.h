#include <system.h>
#include <core/io.h>
#include <core/serial.h>
#include <string.h>
#include "../mpx_supt.h"
#include "comhand.h"
#include "time.h"
#include "date.h"

char commands[11][30] = {
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
	"show-ready"
};
