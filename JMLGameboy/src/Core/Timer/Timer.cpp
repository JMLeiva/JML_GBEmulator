#include "Timer.h"
#include "../CPU/CPU.h"

#define DIVIDER_ADDRESS 0xFF04
#define TIMA_ADDRESS	0xFF05
#define TMA_ADDRESS		0xFF06
#define TAC_ADDRESS		0xFF07

#define SPEED_4K	4096
#define SPEED_262K	262144
#define SPEED_65K	65536
#define SPEED_16K	16384

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
			cyclesOverflow = CPU::TARGET_CPU_SPEED / SPEED_4K;
			break;
		case TimerSpeed::TS_16K:
			cyclesOverflow = CPU::TARGET_CPU_SPEED / SPEED_16K;
			break;
		case TimerSpeed::TS_65K:
			cyclesOverflow = CPU::TARGET_CPU_SPEED / SPEED_65K;
			break;
		case TimerSpeed::TS_262K:
			cyclesOverflow = CPU::TARGET_CPU_SPEED / SPEED_262K;
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
