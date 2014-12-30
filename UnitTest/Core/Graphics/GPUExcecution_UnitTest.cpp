#ifdef UNIT_TEST_ON

#include "../../stdafx.h"
#include "Metadata\Configuration.h"
#include "Core\Graphics\GPU.h"
#include "..\Memory\MemoryElementTest.h"


BOOST_AUTO_TEST_SUITE(GPU_EXCECUTION_Suite)

BOOST_AUTO_TEST_CASE(GPURegsiterFullFrame_Test)
{
	CPU* cpu = new CPU();
	GPU* gpu = new GPU(cpu);

	MemoryElementSmall memorySmall;

	MemoryController::Shared()->AppendMemoryElement(&memorySmall);
	MemoryController::Shared()->AppendMemoryElement(cpu);
	MemoryController::Shared()->AppendMemoryElement(gpu);

	for(int i = 0; i < 144; i++)
	{
		BOOST_CHECK(gpu->LY == i);

		//Searh OAM
		BOOST_CHECK(gpu->STAT_Mode() == GPUState::SEARCHING_OAM);

		//Transfer Data
		gpu->RunCycle(80);
		BOOST_CHECK(gpu->STAT_Mode() == GPUState::TRANSFERING_DATA);

		// Ram Access
		gpu->RunCycle(172);
		BOOST_CHECK(gpu->STAT_Mode() == GPUState::RAM_ACCESS);

		gpu->RunCycle(204);

	}

	// V-Blank
	for(int i = 0; i < 10; i++)
	{
		BOOST_CHECK(gpu->LY == i + 144);

		BOOST_CHECK(gpu->STAT_Mode() == GPUState::VERTICAL_BLANK);
		gpu->RunCycle(456);
	}

	// Oam Again
	BOOST_CHECK(gpu->STAT_Mode() == GPUState::SEARCHING_OAM);


	//V-Blank Interrupt Now
	int ownerId = cpu->GetOwnershipId();
	cpu->IME = true;
	cpu->IE = 0xFF;
	cpu->RunCycle(ownerId);

	BOOST_CHECK(cpu->PC == INTERRUPT_ADDRESS::IA_V_BLANK);

	MemoryController::Shared()->Clear();

	delete gpu;
	delete cpu;
}

BOOST_AUTO_TEST_SUITE_END()

#endif