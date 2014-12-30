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