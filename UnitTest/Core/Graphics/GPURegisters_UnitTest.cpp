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

#ifdef UNIT_TEST_ON

#include "../../stdafx.h"
#include "Metadata\Configuration.h"
#include "Core\Graphics\GPU.h"
#include "..\Memory\MemoryElementTest.h"


BOOST_AUTO_TEST_SUITE(GPU_REGISTERS_Suite)


BOOST_AUTO_TEST_CASE(GPURegsiter_LCDC_Test)
{
	CPU* cpu = new CPU();
	GPU* gpu = new GPU(cpu);

	MemoryController::Shared()->AppendMemoryElement(cpu);
	MemoryController::Shared()->AppendMemoryElement(gpu);

	
	MemoryController::Shared()->WriteMemory(0xFF40, 0xCC);
	BYTE out = MemoryController::Shared()->ReadMemory(0xFF40);

	//General
	BOOST_CHECK(out == 0xCC); //1100 1100

	//LCDC_ObjOn
	BOOST_CHECK(!gpu->LCDC_ObjOn());

	//LCDC_BgOn
	BOOST_CHECK(!gpu->LCDC_BgOn());

	//LCDC_Obj8x8
	BOOST_CHECK(gpu->LCDC_Obj8x8());

	//LCDC_BgAreaLow
	BOOST_CHECK(gpu->LCDC_BgAreaLow());

	//LCDC_BgCharacterLow
	BOOST_CHECK(!gpu->LCDC_BgCharacterLow());

	//LCDC_WindowingOn
	BOOST_CHECK(!gpu->LCDC_WindowingOn());

	//LCDC_WindowAreaLow
	BOOST_CHECK(gpu->LCDC_WindowAreaLow());

	//LCDC_DisplayOn
	BOOST_CHECK(gpu->LCDC_DisplayOn());

	MemoryController::Shared()->Clear();

	delete gpu;
	delete cpu;
}

BOOST_AUTO_TEST_CASE(GPURegsiter_STAT_Test)
{
	CPU* cpu = new CPU();
	GPU* gpu = new GPU(cpu);

	MemoryController::Shared()->AppendMemoryElement(cpu);
	MemoryController::Shared()->AppendMemoryElement(gpu);

	MemoryController::Shared()->WriteMemory(0xFF41, 0xC4);
	BYTE out = MemoryController::Shared()->ReadMemory(0xFF41);

	//General
	BOOST_CHECK(out == 0xC4); //1100 0100

	//STAT_Mode
	BOOST_CHECK(gpu->STAT_Mode() == GPUState::RAM_ACCESS);

	//STAT_LYCMatch
	BOOST_CHECK(gpu->STAT_LYCMatch());

	//STAT_InterruptSelection
	BOOST_CHECK(gpu->STAT_InterruptSelection() == InterruptSelection::IS_MODE_LCY_MATCH);

	MemoryController::Shared()->Clear();

	delete gpu;
	delete cpu;
}

BOOST_AUTO_TEST_CASE(GPURegsiter_SCX_Test)
{
	CPU* cpu = new CPU();
	GPU* gpu = new GPU(cpu);

	MemoryController::Shared()->AppendMemoryElement(cpu);
	MemoryController::Shared()->AppendMemoryElement(gpu);

	MemoryController::Shared()->WriteMemory(0xFF43, 0xC4);
	
	//General
	BOOST_CHECK(gpu->SCX == 0xC4);

	MemoryController::Shared()->Clear();

	delete gpu;
	delete cpu;
}

BOOST_AUTO_TEST_CASE(GPURegsiter_SCY_Test)
{
	CPU* cpu = new CPU();
	GPU* gpu = new GPU(cpu);

	MemoryController::Shared()->AppendMemoryElement(cpu);
	MemoryController::Shared()->AppendMemoryElement(gpu);

	MemoryController::Shared()->WriteMemory(0xFF42, 0xFF);

	//General
	BOOST_CHECK(gpu->SCY == 0xFF);

	MemoryController::Shared()->Clear();

	delete gpu;
	delete cpu;
}

BOOST_AUTO_TEST_CASE(GPURegsiter_BGP_Test)
{
	CPU* cpu = new CPU();
	GPU* gpu = new GPU(cpu);

	MemoryController::Shared()->AppendMemoryElement(cpu);
	MemoryController::Shared()->AppendMemoryElement(gpu);

	MemoryController::Shared()->WriteMemory(0xFF47, 0x0F);

	//General
	BOOST_CHECK(gpu->BGP == 0x0F);

	MemoryController::Shared()->Clear();

	delete gpu;
	delete cpu;
}

BOOST_AUTO_TEST_CASE(GPURegsiter_OBP0_Test)
{
	CPU* cpu = new CPU();
	GPU* gpu = new GPU(cpu);

	MemoryController::Shared()->AppendMemoryElement(cpu);
	MemoryController::Shared()->AppendMemoryElement(gpu);

	MemoryController::Shared()->WriteMemory(0xFF48, 0xF0);

	//General
	BOOST_CHECK(gpu->OBP0 == 0xF0);

	MemoryController::Shared()->Clear();

	delete gpu;
	delete cpu;
}

BOOST_AUTO_TEST_CASE(GPURegsiter_OBP1_Test)
{
	CPU* cpu = new CPU();
	GPU* gpu = new GPU(cpu);

	MemoryController::Shared()->AppendMemoryElement(cpu);
	MemoryController::Shared()->AppendMemoryElement(gpu);

	MemoryController::Shared()->WriteMemory(0xFF49, 0xA0);

	//General
	BOOST_CHECK(gpu->OBP1 == 0xA0);

	MemoryController::Shared()->Clear();

	delete gpu;
	delete cpu;
}

BOOST_AUTO_TEST_CASE(GPURegsiter_BCPS_Test)
{
	CPU* cpu = new CPU();
	GPU* gpu = new GPU(cpu);

	MemoryController::Shared()->AppendMemoryElement(cpu);
	MemoryController::Shared()->AppendMemoryElement(gpu);

	MemoryController::Shared()->WriteMemory(0xFF49, 0xA0);

	//General
	BOOST_CHECK(gpu->OBP1 == 0xA0);

	MemoryController::Shared()->Clear();

	delete gpu;
	delete cpu;
}

BOOST_AUTO_TEST_CASE(GPURegsiter_WX_Test)
{
	CPU* cpu = new CPU();
	GPU* gpu = new GPU(cpu);

	MemoryController::Shared()->AppendMemoryElement(cpu);
	MemoryController::Shared()->AppendMemoryElement(gpu);

	MemoryController::Shared()->WriteMemory(0xFF4B, 0x0F);

	//General
	BOOST_CHECK(gpu->WX == 0x0F);

	MemoryController::Shared()->Clear();

	delete gpu;
	delete cpu;
}

BOOST_AUTO_TEST_CASE(GPURegsiter_WY_Test)
{
	CPU* cpu = new CPU();
	GPU* gpu = new GPU(cpu);

	MemoryController::Shared()->AppendMemoryElement(cpu);
	MemoryController::Shared()->AppendMemoryElement(gpu);

	MemoryController::Shared()->WriteMemory(0xFF4A, 0x1D);

	//General
	BOOST_CHECK(gpu->WY == 0x1D);

	MemoryController::Shared()->Clear();

	delete gpu;
	delete cpu;
}

BOOST_AUTO_TEST_SUITE_END()

#endif