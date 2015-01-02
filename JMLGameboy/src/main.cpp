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

#include "Core\Cartidge\Cartidge.h"
#include "Core\Cartidge\CartidgeBuilder.h"
#include "Core\Graphics\GPU.h"
#include "Core\CPU\CPU.h"
#include "Core\Timer\Timer.h"
#include "Core\Memory\WorkingRAM.h"
#include "Core\Sound\SoundEngine.h"

#ifndef UNIT_TEST_ON

//char* PATH = "C:/Users/Juan/Documents/JML_GBEmulator/ROMs/Super Mario Land.gb";
char* PATH = "C:/Users/Juan/Documents/JML_GBEmulator/ROMs/game_2xy.gb";
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
	SoundEngine* soundEngine = new SoundEngine();
	WorkingRam* workingRam = new WorkingRam();

	MemoryController::Shared()->AppendMemoryElement(cartidge);
	MemoryController::Shared()->AppendMemoryElement(cpu);
	MemoryController::Shared()->AppendMemoryElement(timer);
	MemoryController::Shared()->AppendMemoryElement(gpu);
	MemoryController::Shared()->AppendMemoryElement(workingRam);
	MemoryController::Shared()->AppendMemoryElement(soundEngine);

	int ownerId = cpu->GetOwnershipId();

	for (;;)
	{
		BYTE cyclesCount = cpu->RunCycle(ownerId);
		timer->RunCycle(cyclesCount);
		gpu->RunCycle(cyclesCount);

		if(gpu->Closed())
		{
			break;
		}
	}

	MemoryController::Shared()->Clear();
	
	delete gpu;
	delete timer;
	delete cpu;
	delete cartidge;

	return 0;
}

#endif