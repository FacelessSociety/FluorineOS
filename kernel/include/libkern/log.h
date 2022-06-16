#pragma once

#include <stddef.h>
#include <stdarg.h>

#include <limine.h>

#include "string.h"



// get terminal
static volatile struct limine_terminal_request terminal_request = {
    .id = LIMINE_TERMINAL_REQUEST,
    .revision = 0
};

void putchar(char chr){
	struct limine_terminal *terminal = terminal_request.response->terminals[0];
	
	// write the data out
	terminal_request.response->write(terminal, &chr, strlen(&chr));
}


void puts(char* s){
	struct limine_terminal *terminal = terminal_request.response->terminals[0];
	// write the data out
	terminal_request.response->write(terminal, s, strlen(s));
}

// bug: if \n is given at the end
// it prints every printable ascii character
void printf(char* fmt, ...){
	char* traverse;
	unsigned int i;
	char* s;

	// init args
	va_list arg;
	va_start(arg, fmt);

	for(traverse = fmt; *traverse != '\0'; traverse++){
		// if not a control
		while(*traverse != '%'){
			putchar(*traverse);
			if(*traverse == '\n') puts("\n");
			traverse++;
		}

		traverse++;

		// check for args
		switch(*traverse){
			// character
			// TODO: i dont think it works
			case 'c':
				i = va_arg(arg, int);
				putchar(i);
				break;

			case 'd':
				i = va_arg(arg, int);
				// neg
				if(i < 0){
					i = -i;
					putchar('-');
				}
				puts(convert(i, 10)); // integers are base 10 generally
				break;				

			// string
			case 's':
				s = va_arg(arg, char *);
				puts(s);
				break;
			
			// hex
			case 'x':
				i = va_arg(arg, unsigned int);
				puts(convert(i, 16));
				break;
		}
	}

	// clean up
	va_end(arg);
}