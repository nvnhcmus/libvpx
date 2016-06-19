#pragma once

#include "logHeader.h"

#include <time.h>
#include <stdio.h>
#include <stdarg.h>

// auto add new line
void printLog(const char* logLevel, char * format, ...)
{
	//char buffer[1024];

	//time_t rawtime;
	//struct tm * timeinfo;

	//time(&rawtime);
	//timeinfo = localtime(&rawtime);

	//va_list args;
	//va_start(args, format);
	//vsnprintf(buffer, 1024, format, args);

	////do something with the error
	//fprintf(stdout, "%s:%s\n", logLevel, buffer);

	//va_end(args);
}