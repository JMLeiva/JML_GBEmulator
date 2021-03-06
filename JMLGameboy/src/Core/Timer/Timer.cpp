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

#include "Timer.h"
#include "../CPU/CPU.h"

#define DIVIDER_ADDRESS 0xFF04
#define TIMA_ADDRESS	0xFF05
#define TMA_ADDRESS		0xFF06
#define TAC_ADDRESS		0xFF07

#define SPEED_4K	1024;	//2^10
#define SPEED_262K	16;		//2^4
#define SPEED_65K	64;		//2^6 
#define SPEED_16K	256;	//2^8

#define DIV_MIN		256

Timer::Timer(CPU* cpu)
{
	timerCycles = 0;
	divCycles = 0;

	divider = 0x00;
	tima = 0x00;
	tma = 0x00;
	tac = 0x00;

	running = false;

	this->cpu = cpu;
}

Timer::~Timer()
{

}

bool Timer::Write(const WORD &address, const BYTE &value)
{
	switch(address)
	{
	case DIVIDER_ADDRESS:
		divider = 0x00;
		return true;
	case TIMA_ADDRESS:
		tima = value; //?????
		return true;
	case TMA_ADDRESS:
		tma = value; //????
		return true;
	case TAC_ADDRESS:
		tac = value;

		BYTE speedCode		= value & 0x03;
		BYTE runningCode	= (value >> 2) & 0x01;

		switch(speedCode)
		{
		case TimerSpeed::TS_4K:
			cyclesOverflow = SPEED_4K;
			break;
		case TimerSpeed::TS_16K:
			cyclesOverflow = SPEED_16K;
			break;
		case TimerSpeed::TS_65K:
			cyclesOverflow = SPEED_65K;
			break;
		case TimerSpeed::TS_262K:
			cyclesOverflow = SPEED_262K;
			break;
		}

		running = runningCode != 0;
		return true;
	}

	return false;
}

bool Timer::Read(const WORD &address, BYTE &out)
{
	switch(address)
	{
	case DIVIDER_ADDRESS:
		out = divider;
		return true;
	case TIMA_ADDRESS:
		out = tima;
		return true;
	case TMA_ADDRESS:
		out = tma;
		return true;
	case TAC_ADDRESS:
		out = tac;
		return true;
	}

	return false;
}

void Timer::RunCycle(BYTE cycleCount)
{
	if((tac & 0x04) == 0x00)
	{
		//Timer Not Enabled
		timerCycles = 0;
		return;
	}

	timerCycles += cycleCount;
	
	// Aca se romperia todo si llegara un numero loco (muy grande)
	// Pero el CPU nunca devuelve mas de 4
	if(timerCycles >= cyclesOverflow)
	{
		if(tima == 0xFF)
		{
			tima = tma;
			cpu->SetInterrupt(INTERRUPT_FLAG::IF_TIMER_OVERFLOW);
		}
		else
		{
			tima++;
		}

		timerCycles -= cyclesOverflow;
	}

	divCycles += cycleCount;
	
	// Aca se romperia todo si llegara un numero loco (muy grande)
	// Pero el CPU nunca devuelve mas de 4
	if(divCycles >= DIV_MIN)
	{
		divider++;
		divCycles -= DIV_MIN;
	}
}
