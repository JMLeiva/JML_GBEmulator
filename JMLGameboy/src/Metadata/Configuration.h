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

#ifndef JML_CONFIGURATION
#define JML_CONFIGURATION

#define DEBUGGER_ON
#define UNIT_TEST_ON

#define LOG_LEVEL_CRITICAL	0
#define LOG_LEVEL_ERROR		1
#define LOG_LEVEL_WARNING	2
#define LOG_LEVEL_INFO		3
#define LOG_LEVEL_VERBOSE   4

#ifdef UNIT_TEST_ON
#define CURRENT_LOG_LEVEL	LOG_LEVEL_WARNING
#else
#define CURRENT_LOG_LEVEL	LOG_LEVEL_WARNING
#endif

#define BOOTSTRAP_ENABLED 0 

#endif //JML_CONFIGURATION