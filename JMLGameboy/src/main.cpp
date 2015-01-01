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