#ifndef JML_CONSOLE
#define JML_CONSOLE

#include "../Metadata/Configuration.h"

#ifdef DEBUGGER_ON

void WriteLineC(const char* fmt, ...);
void WriteLineE(const char* fmt, ...);
void WriteLineW(const char* fmt, ...);
void WriteLineI(const char* fmt, ...);
void WriteLineV(const char* fmt, ...);

#endif

#endif 