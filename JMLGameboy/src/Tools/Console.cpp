/*
JML_GBEmulator
Copyright (C) 2015 Leiva Juan Martin

This file is part of JML_GBEmulator.

JML_GBEmulator is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

JML_GBEmulator is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with JML_GBEmulator.  If not, see <http://www.gnu.org/licenses/>.

*/

#include "Console.h"
#include "../Metadata/Configuration.h"
#include <stdio.h>
#include <stdarg.h>

void WriteLine(const char* fmt, va_list args)
{
	char buffer[256];
	vsprintf(buffer, fmt, args);
	printf(buffer);
	printf("\n");
}


void WriteLineC(const char* fmt, ...)
{
#ifdef DEBUGGER_ON
	va_list args;
	va_start(args, fmt);
	WriteLine(fmt, args);
	va_end(args);
#endif
}

void WriteLineE(const char* fmt, ...)
{
#ifdef DEBUGGER_ON
#if CURRENT_LOG_LEVEL >= LOG_LEVEL_ERROR
	va_list args;
	va_start(args, fmt);
	WriteLine(fmt, args);
	va_end(args);
#endif
#endif
}

void WriteLineW(const char* fmt, ...)
{
#ifdef DEBUGGER_ON
#if CURRENT_LOG_LEVEL >= LOG_LEVEL_WARNING
	va_list args;
	va_start(args, fmt);
	WriteLine(fmt, args);
	va_end(args);
#endif
#endif
}

void WriteLineI(const char* fmt, ...)
{
#ifdef DEBUGGER_ON
#if CURRENT_LOG_LEVEL >= LOG_LEVEL_INFO
	va_list args;
	va_start(args, fmt);
	WriteLine(fmt, args);
	va_end(args);
#endif
#endif
}

void WriteLineV(const char* fmt, ...)
{
#ifdef DEBUGGER_ON
#if CURRENT_LOG_LEVEL >= LOG_LEVEL_VERBOSE
	va_list args;
	va_start(args, fmt);
	WriteLine(fmt, args);
	va_end(args);
#endif
#endif
}


