#include <system.h>
#include <core/io.h>
#include <core/serial.h>
#include "../modules/mpx_supt.h"
#include "colortext.h"

void write_text_red(char str[]){
	int size = 10;
	int str_size = sizeof(str);
	sys_req(WRITE, DEFAULT_DEVICE, "\033[0;31m", &size);
	sys_req(WRITE, DEFAULT_DEVICE, str, &str_size);
	sys_req(WRITE, DEFAULT_DEVICE, "\033[0m", &size);
}

void write_text_bold_red(char str[]){
	int size = 10;
	int str_size = sizeof(str);
	sys_req(WRITE, DEFAULT_DEVICE, "\033[1;31m", &size);
	sys_req(WRITE, DEFAULT_DEVICE, str, &str_size);
	sys_req(WRITE, DEFAULT_DEVICE, "\033[0m", &size);
}

void write_text_green(char str[]){
	int size = 10;
	int str_size = sizeof(str);
	sys_req(WRITE, DEFAULT_DEVICE, "\033[0;32m", &size);
	sys_req(WRITE, DEFAULT_DEVICE, str, &str_size);
	sys_req(WRITE, DEFAULT_DEVICE, "\033[0m", &size);
}

void write_text_bold_green(char str[]){
	int size = 10;
	int str_size = sizeof(str);
	sys_req(WRITE, DEFAULT_DEVICE, "\033[1;32m", &size);
	sys_req(WRITE, DEFAULT_DEVICE, str, &str_size);
	sys_req(WRITE, DEFAULT_DEVICE, "\033[0m", &size);
}

void write_text_yellow(char str[]){
	int size = 10;
	int str_size = sizeof(str);
	sys_req(WRITE, DEFAULT_DEVICE, "\033[0;33m", &size);
	sys_req(WRITE, DEFAULT_DEVICE, str, &str_size);
	sys_req(WRITE, DEFAULT_DEVICE, "\033[0m", &size);
}

void write_text_bold_yellow(char str[]){
	int size = 10;
	int str_size = sizeof(str);
	sys_req(WRITE, DEFAULT_DEVICE, "\033[1;33m", &size);
	sys_req(WRITE, DEFAULT_DEVICE, str, &str_size);
	sys_req(WRITE, DEFAULT_DEVICE, "\033[0m", &size);
}

void write_text_blue(char str[]){
	int size = 10;
	int str_size = sizeof(str);
	sys_req(WRITE, DEFAULT_DEVICE, "\033[0;34m", &size);
	sys_req(WRITE, DEFAULT_DEVICE, str, &str_size);
	sys_req(WRITE, DEFAULT_DEVICE, "\033[0m", &size);
}

void write_text_bold_blue(char str[]){
	int size = 10;
	int str_size = sizeof(str);
	sys_req(WRITE, DEFAULT_DEVICE, "\033[1;34m", &size);
	sys_req(WRITE, DEFAULT_DEVICE, str, &str_size);
	sys_req(WRITE, DEFAULT_DEVICE, "\033[0m", &size);
}

void write_text_magenta(char str[]){
	int size = 10;
	int str_size = sizeof(str);
	sys_req(WRITE, DEFAULT_DEVICE, "\033[0;35m", &size);
	sys_req(WRITE, DEFAULT_DEVICE, str, &str_size);
	sys_req(WRITE, DEFAULT_DEVICE, "\033[0m", &size);
}

void write_text_bold_magenta(char str[]){
	int size = 10;
	int str_size = sizeof(str);
	sys_req(WRITE, DEFAULT_DEVICE, "\033[1;35m", &size);
	sys_req(WRITE, DEFAULT_DEVICE, str, &str_size);
	sys_req(WRITE, DEFAULT_DEVICE, "\033[0m", &size);
}

void write_text_cyan(char str[]){
	int size = 10;
	int str_size = sizeof(str);
	sys_req(WRITE, DEFAULT_DEVICE, "\033[0;36m", &size);
	sys_req(WRITE, DEFAULT_DEVICE, str, &str_size);
	sys_req(WRITE, DEFAULT_DEVICE, "\033[0m", &size);
}

void write_text_bold_cyan(char str[]){
	int size = 10;
	int str_size = sizeof(str);
	sys_req(WRITE, DEFAULT_DEVICE, "\033[1;36m", &size);
	sys_req(WRITE, DEFAULT_DEVICE, str, &str_size);
	sys_req(WRITE, DEFAULT_DEVICE, "\033[0m", &size);
}
