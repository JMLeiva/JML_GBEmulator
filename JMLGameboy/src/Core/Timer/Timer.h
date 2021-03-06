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

#ifndef JML_TIMER
#define JML_TIMER

#include "../Memory/MemoryElement.h"

class CPU;

enum TimerSpeed
{
	TS_4K	= 0,
	TS_262K	= 1,
	TS_65K	= 2,
	TS_16K	= 3,
};

class Timer : public MemoryElement
{
public:
	Timer(CPU* cpu);
	~Timer();
	bool Write(const WORD &address, const BYTE &value);
	bool Read(const WORD &address, BYTE &out);

	void RunCycle(BYTE cycleCount);

private:
	bool running;
	unsigned int cyclesOverflow;

	unsigned int timerCycles;
	unsigned int divCycles;

	CPU* cpu;

	BYTE divider;
	BYTE tima;
	BYTE tma;
	BYTE tac;
};

#endif