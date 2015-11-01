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

#include "Metadata\Configuration.h"
#include "Tools\Debugger\Debugger.h"

#include "Core\Cartidge\Cartidge.h"
#include "Core\Cartidge\CartidgeBuilder.h"
#include "Core\Graphics\GPU.h"
#include "Core\CPU\CPU.h"
#include "Core\Timer\Timer.h"
#include "Core\Memory\WorkingRAM.h"
#include "Core\Sound\SoundEngine.h"
#include "Core\Input\Joypad.h"
#include <thread>

#ifndef UNIT_TEST_ON

char* PATH = "C:/Users/Juan/Documents/C++/JML_GBEmulator/ROMs/Super Mario Land.gb";
//char* PATH = "C:/Users/Juan/Documents/C++/JML_GBEmulator/ROMs/Tests/02-interrupts.gb";
//char* PATH = "C:/Users/Juan/Documents/C++/JML_GBEmulator/ROMs/Tests/cpu_instrs.gb";


#define SFML_POLL_INTERVAL			100

unsigned int eventCycles = 0;
bool closed = false;

#ifndef UNIT_TEST_ON
sf::RenderWindow* window;
sf::Event event;
#endif

void UpdateEvents(Joypad* joypad)
{
#ifndef UNIT_TEST_ON
	while(window->pollEvent(event))
	{
		if(event.type == sf::Event::Closed)
		{
			window->close();
			closed = true;
		}
		else
		{
			joypad->HandleEvent(event);
		}
	}
#endif
}

#ifdef DEBUGGER_ON
Debugger debugger;

void runDebugger(CPU* cpu, int argc, char **argv)
{
	debugger.Initialize(cpu, argc, argv);
}
#endif

int main(int argc, char** argv)
{
	Cartidge* cartidge = CartidgeBuilder().Build(PATH);


	if(cartidge == NULL)
	{
		throw std::runtime_error("Cartidge Error");
	}

	CPU* cpu = new CPU();
	Timer* timer = new Timer(cpu);
	GPU* gpu = new GPU(cpu);
	window = gpu->GetWindow();
	SoundEngine* soundEngine = new SoundEngine();
	WorkingRam* workingRam = new WorkingRam();
	Joypad* joypad = new Joypad();

	MemoryController::Shared()->AppendMemoryElement(cartidge);
	MemoryController::Shared()->AppendMemoryElement(cpu);
	MemoryController::Shared()->AppendMemoryElement(timer);
	MemoryController::Shared()->AppendMemoryElement(gpu);
	MemoryController::Shared()->AppendMemoryElement(workingRam);
	MemoryController::Shared()->AppendMemoryElement(soundEngine);
	MemoryController::Shared()->AppendMemoryElement(joypad);

	int ownerId = cpu->GetOwnershipId();

#ifdef DEBUGGER_ON
	//Thread?
	std::thread thread(runDebugger, cpu, argc, argv);
#endif

	for (;;)
	{

#ifdef DEBUGGER_ON
		debugger.Update();
#endif

		BYTE cyclesCount = cpu->RunCycle(ownerId);
		timer->RunCycle(cyclesCount);
		gpu->RunCycle(cyclesCount);

		// SFML Events 
		eventCycles += cyclesCount;
		if(eventCycles >= SFML_POLL_INTERVAL)
		{
			eventCycles = 0;
			UpdateEvents(joypad);
		}

		if(closed)
		{
			break;
		}
	}

	MemoryController::Shared()->Clear();
	
	delete gpu;
	delete timer;
	delete cpu;
	delete cartidge;
	delete joypad;

	return 0;
}

#endif